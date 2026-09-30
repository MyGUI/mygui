#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_DataStream.h"
#include "MyGUI_DataFileStream.h"
#include "MyGUI_DataManager.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{

	using unittest::require;

	using unittest::TemporaryFile;

	// Exercise the same public API through both borrowed memory and owned file streams.
	class StreamFixture
	{
	public:
		StreamFixture(const std::string& _content, bool _file)
		{
			if (_file)
			{
				file = std::make_unique<TemporaryFile>(_content);
				auto source = std::make_unique<std::ifstream>(file->path(), std::ios::binary);
				require(source->is_open(), "The temporary stream fixture must be readable");
				input = source.get();
				stream = std::make_unique<MyGUI::DataFileStream>(std::move(source));
				require(source == nullptr, "DataFileStream must take ownership of the file stream");
			}
			else
			{
				memory = std::make_unique<std::istringstream>(_content);
				input = memory.get();
				stream = std::make_unique<MyGUI::DataStream>(input);
			}
		}

		std::unique_ptr<TemporaryFile> file;
		std::unique_ptr<std::istringstream> memory;
		std::istream* input{nullptr};
		std::unique_ptr<MyGUI::IDataStream> stream;
	};

	void checkNullStream(MyGUI::IDataStream& _stream)
	{
		char buffer = '!';
		std::string line = "unchanged";
		require(_stream.size() == 0 && _stream.eof(), "A null stream must be empty and at EOF");
		require(_stream.read(&buffer, 1) == 0 && buffer == '!', "Null reads must leave the buffer untouched");
		require(_stream.read(nullptr, 0) == 0, "Zero-byte null reads must be safe");
		_stream.readline(line);
		require(line == "unchanged", "A null stream must preserve the line output");
	}

	void testNullStreams()
	{
		MyGUI::DataStream empty;
		MyGUI::DataStream null(nullptr);
		MyGUI::DataFileStream emptyFile;
		MyGUI::DataFileStream nullFile{std::unique_ptr<std::ifstream>()};
		checkNullStream(empty);
		checkNullStream(null);
		checkNullStream(emptyFile);
		checkNullStream(nullFile);
	}

	void testEmptyStreams()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture("", file);
			auto& stream = *fixture.stream;
			require(stream.size() == 0 && stream.size() == 0, "Empty stream size must be stable");
			require(!stream.eof(), "An untouched empty stream must not report EOF before a read");
			char buffer = '!';
			require(stream.read(&buffer, 1) == 0 && buffer == '!', "An empty read must return zero without writing");
			require(stream.eof(), "An empty read must detect EOF");
			std::string line;
			stream.readline(line);
			require(line.empty(), "Reading a line at EOF must not invent content");
		}
	}

	void testBinaryReads()
	{
		const std::string content("a\0b\r\nc\xff", 7);
		for (bool file : {false, true})
		{
			StreamFixture fixture(content, file);
			auto& stream = *fixture.stream;
			std::array<char, 16> buffer;
			buffer.fill('!');
			require(stream.read(buffer.data(), 2) == 2, "The first read must return its byte count");
			require(std::string(buffer.data(), 2) == content.substr(0, 2), "Reads must preserve embedded NUL bytes");
			require(
				stream.read(buffer.data(), buffer.size()) == 5,
				"A partial final read must return only remaining bytes");
			require(
				std::string(buffer.data(), 5) == content.substr(2),
				"Partial reads must continue at the current position");
			require(buffer[5] == '!', "Partial reads must not write beyond the returned byte count");
			require(stream.eof(), "An over-sized read must set EOF");
			buffer.fill('!');
			require(stream.read(buffer.data(), 1) == 0 && buffer[0] == '!', "Reads after EOF must return zero");
			require(stream.read(buffer.data(), 1) == 0, "Repeated EOF reads must remain empty");
		}
	}

	void testExactAndZeroReads()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture("abc", file);
			auto& stream = *fixture.stream;
			char buffer[3]{};
			require(stream.read(nullptr, 0) == 0 && fixture.input->tellg() == 0, "Zero reads must not advance input");
			require(
				stream.read(buffer, 3) == 3 && std::string(buffer, 3) == "abc",
				"Exact reads must return all bytes");
			require(!stream.eof(), "An exact read need not detect EOF until the next read");
			require(stream.read(nullptr, 0) == 0 && !stream.eof(), "Zero reads must preserve stream state");
			require(stream.read(buffer, 1) == 0 && stream.eof(), "The next read after an exact read must detect EOF");
		}
	}

	void testChunkedReads()
	{
		std::string content;
		for (size_t index = 0; index < 10003; ++index)
			content += static_cast<char>(index % 256);
		for (bool file : {false, true})
		{
			StreamFixture fixture(content, file);
			std::string result;
			char buffer[4096];
			for (size_t expected : {size_t(4096), size_t(4096), size_t(1811), size_t(0)})
			{
				size_t count = fixture.stream->read(buffer, sizeof(buffer));
				require(count == expected, "Chunked reads must report each full chunk, the tail and EOF accurately");
				result.append(buffer, count);
			}
			require(result == content, "Reading to EOF must reproduce the entire binary payload exactly");
		}
	}

	void testSizePreservesPosition()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture("abcdef", file);
			fixture.input->seekg(2);
			auto& stream = *fixture.stream;
			require(stream.size() == 6, "Size must describe the entire stream, not the remaining bytes");
			require(fixture.input->tellg() == 2, "The first size query must preserve the current position");
			char buffer[2];
			require(
				stream.read(buffer, 2) == 2 && std::string(buffer, 2) == "cd",
				"Reads must respect an initial offset");
			require(
				stream.size() == 6 && fixture.input->tellg() == 4,
				"Cached size queries must preserve the current position");
			fixture.input->seekg(1);
			require(stream.size() == 6 && fixture.input->tellg() == 1, "External seeks must not change total size");
		}
	}

	void testSizeAfterReading()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture("abc", file);
			char buffer[8];
			require(fixture.stream->read(buffer, sizeof(buffer)) == 3, "Reading through EOF must report actual bytes");
			auto state = fixture.input->rdstate();
			require(fixture.stream->size() == 3, "The first size query must still work after EOF");
			require(fixture.input->rdstate() == state, "Size must preserve EOF and failure flags");
			fixture.input->clear();
			require(fixture.input->tellg() == 3, "Size at EOF must not rewind input");
			fixture.input->seekg(0);
			require(
				fixture.stream->read(buffer, 1) == 1 && buffer[0] == 'a',
				"The borrowed stream must remain recoverable");
		}
	}

	void testLinesAndMixedReads()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture("first\n\nsecond|tail", file);
			auto& stream = *fixture.stream;
			std::string line;
			stream.readline(line);
			require(line == "first", "The default delimiter must be newline and must be removed");
			require(
				stream.size() == 18 && fixture.input->tellg() == 6,
				"Size after a line read must preserve its position");
			stream.readline(line);
			require(line.empty(), "Consecutive delimiters must produce an empty line");
			char buffer[2];
			require(
				stream.read(buffer, 2) == 2 && std::string(buffer, 2) == "se",
				"Byte reads must continue after line reads");
			stream.readline(line, '|');
			require(line == "cond", "A custom delimiter must continue at the current byte position");
			stream.readline(line);
			require(line == "tail" && stream.eof(), "An unterminated final line must return its content and set EOF");
		}
	}

	void testReadAfterLineWithoutSize()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture("skip\nrest", file);
			std::string line;
			fixture.stream->readline(line);
			char buffer[4];
			require(
				fixture.stream->read(buffer, 4) == 4 && std::string(buffer, 4) == "rest",
				"A byte read must not perform an implicit size query that rewinds the stream");
		}
	}

	void testBinaryDelimiters()
	{
		for (bool file : {false, true})
		{
			StreamFixture fixture(std::string("a\0b\r\n", 5), file);
			std::string line;
			fixture.stream->readline(line, '\0');
			require(line == "a", "NUL must work as a line delimiter");
			fixture.stream->readline(line);
			require(line == "b\r", "Line reads must preserve carriage returns in binary input");
			fixture.stream->readline(line);
			require(
				line.empty() && fixture.stream->eof(),
				"Reading past a trailing delimiter must return an empty line");
		}
	}

	class NonSeekableBuffer : public std::streambuf
	{
	public:
		NonSeekableBuffer()
		{
			setg(content.data(), content.data(), content.data() + content.size());
		}

	private:
		std::array<char, 3> content{'a', 'b', 'c'};
	};

	void testNonSeekableStream()
	{
		NonSeekableBuffer buffer;
		std::istream input(&buffer);
		input.exceptions(std::ios::failbit | std::ios::badbit);
		MyGUI::DataStream stream(&input);
		require(
			stream.size() == 0 && input.good(),
			"Unknown size must return zero without failing a non-seekable stream");
		char data[3];
		require(stream.read(data, 3) == 3 && std::string(data, 3) == "abc", "Reading must not require seeking");
		require(stream.size() == 0 && input.good(), "Repeated unknown-size queries must preserve stream state");
		input.exceptions(std::ios::goodbit);
		require(stream.read(data, 1) == 0 && stream.eof(), "Non-seekable reads must terminate at EOF");
	}

	class UnknownEndBuffer : public std::stringbuf
	{
	public:
		UnknownEndBuffer() :
			std::stringbuf("abc")
		{
		}

	protected:
		pos_type seekoff(off_type _offset, std::ios::seekdir _direction, std::ios::openmode _mode) override
		{
			if (_direction == std::ios::end)
				return pos_type(off_type(-1));
			return std::stringbuf::seekoff(_offset, _direction, _mode);
		}
	};

	void testUnavailableSize()
	{
		std::istream nullInput(nullptr);
		MyGUI::DataStream nullBuffer(&nullInput);
		require(nullBuffer.size() == 0, "A stream without a buffer must have unavailable size");
		char byte = '!';
		require(nullBuffer.read(&byte, 1) == 0 && byte == '!', "A stream without a buffer must not report data");

		UnknownEndBuffer buffer;
		std::istream input(&buffer);
		input.seekg(1);
		MyGUI::DataStream stream(&input);
		require(stream.size() == 0 && input.good(), "An unavailable end position must not corrupt stream state");
		require(input.tellg() == 1, "A failed size query must restore the read position");
		require(stream.read(&byte, 1) == 1 && byte == 'b', "Reads must remain usable when end seeking is unsupported");
	}

	void testFailedStreamsAndExceptions()
	{
		for (bool file : {false, true})
		{
			for (auto state : {std::ios::failbit, std::ios::badbit})
			{
				StreamFixture fixture("abc", file);
				fixture.input->setstate(state);
				require(
					fixture.stream->size() == 3 && fixture.input->rdstate() == state,
					"Size queries must preserve error flags");
				char buffer = '!';
				require(
					fixture.stream->read(&buffer, 1) == 0 && buffer == '!',
					"A failed stream must not claim to have read bytes");
			}
			StreamFixture fixture("abc", file);
			fixture.input->exceptions(std::ios::failbit | std::ios::badbit);
			unittest::requireThrows<std::ios_base::failure>(
				[&]
				{
					char buffer[8];
					fixture.stream->read(buffer, sizeof(buffer));
				},
				"Read failures must honor the underlying stream exception mask");
			require(
				fixture.stream->size() == 3 && fixture.stream->eof(),
				"Size at EOF must preserve an enabled exception mask");
			require(
				fixture.input->exceptions() == (std::ios::failbit | std::ios::badbit),
				"Size queries must leave the exception mask unchanged");
			fixture.input->exceptions(std::ios::goodbit);
		}
	}

	void testBorrowedLifetime()
	{
		std::istringstream input("abc");
		{
			MyGUI::DataStream stream(&input);
			char buffer;
			require(stream.read(&buffer, 1) == 1, "The wrapper must read its borrowed input");
		}
		std::string remaining;
		std::getline(input, remaining);
		require(
			remaining == "bc",
			"Destroying DataStream must leave its borrowed input alive and at the same position");
	}

	class TrackedFileStream : public std::ifstream
	{
	public:
		TrackedFileStream(const std::filesystem::path& _path, bool& _destroyed) :
			std::ifstream(_path, std::ios::binary),
			destroyed(_destroyed)
		{
		}

		~TrackedFileStream() override
		{
			destroyed = true;
		}

	private:
		bool& destroyed;
	};

	void testFileOwnership()
	{
		TemporaryFile file("owned");
		for (bool unwind : {false, true})
		{
			bool destroyed = false;
			try
			{
				std::unique_ptr<std::ifstream> input = std::make_unique<TrackedFileStream>(file.path(), destroyed);
				std::unique_ptr<MyGUI::IDataStream> stream = std::make_unique<MyGUI::DataFileStream>(std::move(input));
				require(input == nullptr && !destroyed, "The file must stay alive under DataFileStream ownership");
				if (unwind)
					throw std::runtime_error("unwind owned stream");
			}
			catch (const std::runtime_error& error)
			{
				require(
					unwind && std::string(error.what()) == "unwind owned stream",
					"Unexpected ownership test failure");
			}
			require(
				destroyed,
				"Destruction through IDataStream must release the owned file, including during unwinding");
		}
	}

	class ChunkStream : public MyGUI::IDataStream
	{
	public:
		ChunkStream(std::string _content, size_t _chunk, bool _throwAfterRead = false) :
			content(std::move(_content)),
			chunk(_chunk),
			throwAfterRead(_throwAfterRead)
		{
		}

		bool eof() override
		{
			require(false, "readAll must use the read result rather than querying EOF");
			return false;
		}

		size_t size() override
		{
			require(false, "readAll must not query the stream size");
			return 0;
		}

		void readline(std::string&, MyGUI::Char) override
		{
			require(false, "readAll must preserve raw bytes rather than reading lines");
		}

		size_t read(void* _buffer, size_t _count) override
		{
			if (throwAfterRead && offset != 0)
				throw std::ios_base::failure("readAll test failure");
			const auto count = std::min({_count, chunk, content.size() - offset});
			std::memcpy(_buffer, content.data() + offset, count);
			offset += count;
			return count;
		}

	private:
		std::string content;
		size_t chunk;
		bool throwAfterRead;
		size_t offset{0};
	};

	bool matchesBytes(const std::vector<std::byte>& _actual, std::string_view _expected)
	{
		return _actual.size() == _expected.size() &&
			(_expected.empty() || std::memcmp(_actual.data(), _expected.data(), _expected.size()) == 0);
	}

	void testReadAllChunksWithoutSize()
	{
		std::string content;
		for (size_t index = 0; index < 10003; ++index)
			content += static_cast<char>(index % 256);
		for (size_t chunk : {size_t(7), content.size()})
		{
			ChunkStream stream(content, chunk);
			require(
				matchesBytes(stream.readAll(), content),
				"readAll must preserve partial binary reads without querying size");
		}
	}

	void testReadAllPositionAndTermination()
	{
		const std::string content("a\0b\r\n", 5);
		std::istringstream input(content);
		input.seekg(1);
		MyGUI::DataStream stream(&input);
		require(matchesBytes(stream.readAll(), content.substr(1)), "readAll must start at the current position");
		require(stream.readAll().empty(), "readAll must return empty after consuming the resource");
		ChunkStream stalled("unread", 0);
		require(stalled.readAll().empty(), "A zero-byte read without EOF must terminate readAll");
	}

	void testReadAllExceptions()
	{
		ChunkStream stream("partial result", 3, true);
		unittest::requireThrows<std::ios_base::failure>(
			[&] { stream.readAll(); },
			"readAll must propagate errors instead of returning a partial result as success");
	}

	void testReadAllTextPreservesBytes()
	{
		const std::string content = std::string("a\0b\r\n", 5) + "\xC3\xA9\xFF";
		ChunkStream stream(content, 2);
		require(stream.readAllText() == content, "readAllText must preserve NULs, CRLF and non-ASCII bytes exactly");
	}

	class TrackingDataManager : public MyGUI::DataManager
	{
	public:
		MyGUI::IDataStream* getData(const std::string& _name) const override
		{
			if (_name == "missing")
				return nullptr;
			if (_name == "open-error")
				throw std::ios_base::failure("getDataHolder test failure");
			if (_name == "read-error")
				return &failing;
			return &stream;
		}

		void freeData(MyGUI::IDataStream* _data) override
		{
			++releases;
			if (_data == nullptr)
				++nullReleases;
			else if (_data != &stream && _data != &failing)
				unexpectedRelease = true;
		}

		bool isDataExist(const std::string&) const override
		{
			return false;
		}

		int releases{0};
		int nullReleases{0};
		bool unexpectedRelease{false};

	private:
		// Manager-owned streams ensure the holder uses freeData(), rather than deleting directly.
		std::istringstream input{"owned"};
		mutable MyGUI::DataStream stream{&input};
		mutable ChunkStream failing{"partial result", 3, true};
	};

	void testGetDataHolderOwnership()
	{
		TrackingDataManager manager;
		{
			const auto data = manager.getDataHolder("data");
			require(data && data.getData() == &*data, "getDataHolder must return an owning holder with pointer access");
			require(data->readAllText() == "owned", "The holder must expose the stream returned by getData");
			require(manager.releases == 0, "The stream must stay owned until the holder is destroyed");
		}
		require(manager.releases == 1 && !manager.unexpectedRelease, "The holder must release its stream exactly once");
		{
			MyGUI::DataStreamHolder legacy = manager.getData("data");
			require(static_cast<bool>(legacy), "Existing raw-pointer adoption must remain available");
		}
		require(
			manager.releases == 2 && !manager.unexpectedRelease,
			"Legacy adoption must release through its manager");
	}

	void testGetDataHolderMissingResource()
	{
		TrackingDataManager manager;
		{
			auto data = manager.getDataHolder("missing");
			require(!data && data.getData() == nullptr, "A missing resource must produce an empty holder");
		}
		require(
			manager.releases == 1 && manager.nullReleases == 1,
			"An empty holder must safely release the null stream");
	}

	void testGetDataHolderExceptions()
	{
		TrackingDataManager manager;
		unittest::requireThrows<std::ios_base::failure>(
			[&]
			{
				auto data = manager.getDataHolder("read-error");
				data->readAll();
			},
			"Read errors must unwind the owning holder");
		require(manager.releases == 1 && !manager.unexpectedRelease, "Read errors must unwind the owning holder");
		unittest::requireThrows<std::ios_base::failure>(
			[&] { auto data = manager.getDataHolder("open-error"); },
			"Open errors must propagate without releasing an unacquired stream");
		require(manager.releases == 1, "Open errors must propagate without releasing an unacquired stream");
	}

	void testUnopenedFile()
	{
		MyGUI::DataFileStream stream(std::make_unique<std::ifstream>());
		require(stream.size() == 0, "An unopened file must report zero instead of an invalid size");
		char buffer = '!';
		require(stream.read(&buffer, 1) == 0 && buffer == '!', "An unopened file must not return data");
	}

}

int main()
{
	return unittest::runTests({
		{"Default and null streams", testNullStreams},
		{"Empty memory and file streams", testEmptyStreams},
		{"Binary and partial reads", testBinaryReads},
		{"Exact and zero-byte reads", testExactAndZeroReads},
		{"Chunked reads through EOF", testChunkedReads},
		{"Size queries preserve position", testSizePreservesPosition},
		{"Size queries after EOF", testSizeAfterReading},
		{"Line delimiters and mixed reads", testLinesAndMixedReads},
		{"Byte reads after lines without a size query", testReadAfterLineWithoutSize},
		{"Binary line delimiters", testBinaryDelimiters},
		{"Non-seekable input", testNonSeekableStream},
		{"Unavailable sizes", testUnavailableSize},
		{"Stream errors and exceptions", testFailedStreamsAndExceptions},
		{"Borrowed stream lifetime", testBorrowedLifetime},
		{"Owned file lifetime", testFileOwnership},
		{"Unopened file", testUnopenedFile},
		{"Read all with partial reads without size queries", testReadAllChunksWithoutSize},
		{"Read all position and termination", testReadAllPositionAndTermination},
		{"Read all exception propagation", testReadAllExceptions},
		{"Read all text preserves bytes", testReadAllTextPreservesBytes},
		{"Owning getDataHolder and raw-pointer adoption", testGetDataHolderOwnership},
		{"Opening missing resources", testGetDataHolderMissingResource},
		{"Owning stream exception cleanup", testGetDataHolderExceptions},
	});
}
