#include "BehaviourTestSupport.h"
#include "MyGUI_DataStream.h"
#include "MyGUI_DataFileStream.h"
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{

	using unittest::require;

	class TemporaryFile
	{
	public:
		explicit TemporaryFile(const std::string& _content)
		{
			path = std::filesystem::temp_directory_path() /
				("mygui_data_stream_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
			std::ofstream output(path, std::ios::binary);
			output.write(_content.data(), static_cast<std::streamsize>(_content.size()));
			require(output.good(), "The temporary stream fixture must be writable");
		}

		~TemporaryFile()
		{
			std::error_code error;
			std::filesystem::remove(path, error);
		}

		std::filesystem::path path;
	};

	// Exercise the same public API through both borrowed memory and owned file streams.
	class StreamFixture
	{
	public:
		StreamFixture(const std::string& _content, bool _file)
		{
			if (_file)
			{
				file = std::make_unique<TemporaryFile>(_content);
				auto source = std::make_unique<std::ifstream>(file->path, std::ios::binary);
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
			bool threw = false;
			try
			{
				char buffer[8];
				fixture.stream->read(buffer, sizeof(buffer));
			}
			catch (const std::ios_base::failure&)
			{
				threw = true;
			}
			require(threw, "Read failures must honor the underlying stream exception mask");
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
				std::unique_ptr<std::ifstream> input = std::make_unique<TrackedFileStream>(file.path, destroyed);
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
	});
}
