#include "BehaviourTestSupport.h"
#include "MyGUI_FileSystemUtility.h"
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{

	using unittest::require;
	using MyGUI::xml::Document;

	void testTreeMutation()
	{
		Document doc;
		auto root = doc.createRoot("Root");
		root->addAttribute("answer", 42);
		root->setAttribute("answer", "43");
		root->setAttribute("escaped", "a&b<\"");
		require(
			root->getAttributes() == MyGUI::xml::VectorAttributes({{"answer", "43"}, {"escaped", "a&b<\""}}),
			"Attribute replacement must preserve order without adding a duplicate");
		std::string value = "sentinel";
		require(
			!root->findAttribute("missing", value) && value == "sentinel",
			"Missing attributes must leave the output unchanged");
		require(
			root->findAttribute("answer", value) && value == "43",
			"Attribute lookup must return the current value");
		root->removeAttribute("answer");
		require(
			root->findAttribute("answer").empty() && root->getAttributes().size() == 1,
			"Attribute removal must preserve other attributes");
		auto first = root->createChild("Item", "first & <text>");
		root->createChild("Other", "skip");
		auto last = root->createChild("Item", "last");
		require(first->getParent()->getName() == "Root", "Children must retain their parent");
		auto filtered = root->getElementEnumerator();
		require(
			filtered.next("Item") && filtered->getContent() == "first & <text>",
			"Filtered enumeration must find its first match");
		require(
			filtered.next("Item") && filtered.current()->getContent() == "last",
			"Filtered enumeration must skip nonmatching siblings");
		require(!filtered.next("Item") && !filtered.next("Item"), "Exhausted enumeration must remain exhausted");
		root->removeChild(last);
		auto all = root->getElementEnumerator();
		require(
			all.next() && all->getName() == "Item" && all.next() && all->getName() == "Other" && !all.next(),
			"Removing one child must preserve sibling order");
		first->createChild("Nested", "old");
		first->setContent("replacement <&>");
		require(first->getContent() == "replacement <&>", "Replacing content must discard nested elements");
		auto nested = first->getElementEnumerator();
		require(!nested.next("Nested"), "setContent must remove the old nested subtree");
		first->addAttribute("flag", "yes");
		first->clear();
		require(
			first->getName() == "Item" && first->getContent().empty() && first->getAttributes().empty(),
			"Clearing an element must preserve its name while removing content and attributes");
	}

	void testCopyLifetimeAndRoundTrip()
	{
		std::unique_ptr<MyGUI::xml::Element> copy;
		{
			Document source;
			auto root = source.createRoot("Root");
			root->addAttribute("id", "original");
			root->createChild("Child", "Unicode \xD0\x91\xF0\x9F\x98\x80 & <text>");
			copy = root->createCopy();
			copy->setAttribute("id", "copy");
			require(
				root->findAttribute("id") == "original",
				"Deep-copy attributes must be independent of the original");
			root->clear();
		}
		require(
			copy->getName() == "Root" && copy->findAttribute("id") == "copy",
			"A deep copy must outlive its original document");
		auto children = copy->getElementEnumerator();
		require(
			children.next("Child") && children->getContent() == "Unicode \xD0\x91\xF0\x9F\x98\x80 & <text>",
			"Deep copies must own the entire descendant subtree");

		Document doc;
		std::istringstream initial("<?xml version=\"1.0\"?><!----><Old><Discard/></Old>");
		require(doc.open(initial), "Initial XML must parse");
		doc.createDeclaration("1.0", "UTF-8");
		doc.createDeclaration("1.0", "UTF-8");
		auto root = doc.createRoot("New");
		root->setAttribute("quoted", "\"<&>");
		root->createChild("Text", children->getContent());
		std::ostringstream saved;
		require(doc.save(saved), "Constructed XML must serialize");
		const auto output = saved.str();
		const auto declaration = output.find("<?xml");
		require(
			declaration != std::string::npos && output.find("<?xml", declaration + 1) == std::string::npos,
			"Replacing declarations must serialize exactly one declaration");
		require(
			output.find("<Old") == std::string::npos && output.find("<!---->") != std::string::npos,
			"Replacing the root must remove old elements while preserving document comments");
		Document restored;
		std::istringstream stream(output);
		require(restored.open(stream), "Serialized XML must parse again");
		require(
			restored.getRoot()->getName() == "New" && restored.getRoot()->findAttribute("quoted") == "\"<&>",
			"Round-trip must preserve the replacement root and escaped attributes");
		auto text = restored.getRoot()->getElementEnumerator();
		require(
			text.next("Text") && text->getContent() == children->getContent(),
			"Round-trip must preserve Unicode and escaped text");
	}

	class ChunkStream : public MyGUI::IDataStream
	{
	public:
		ChunkStream(std::string _text, size_t _reportedSize, size_t _chunk) :
			text(std::move(_text)),
			reportedSize(_reportedSize),
			chunk(_chunk)
		{
		}
		bool eof() override
		{
			return offset == text.size();
		}
		size_t size() override
		{
			return reportedSize;
		}
		void readline(std::string&, MyGUI::Char) override
		{
			require(false, "XML must read bytes, not lines");
		}
		size_t read(void* _buffer, size_t _count) override
		{
			const auto count = std::min({_count, chunk, text.size() - offset});
			std::memcpy(_buffer, text.data() + offset, count);
			offset += count;
			return count;
		}

	private:
		std::string text;
		size_t reportedSize, chunk, offset{0};
	};

	void testDataStreams()
	{
		const std::string xml = "<Root><Text>" + std::string(5000, 'x') + "</Text></Root>";
		for (size_t size : {size_t(0), xml.size() / 2, xml.size(), xml.size() + 100})
		{
			ChunkStream stream(xml, size, 7);
			Document doc;
			require(doc.open(&stream), "Known-size and unknown-size streams must accept partial reads through EOF");
			auto children = doc.getRoot()->getElementEnumerator();
			require(
				children.next("Text") && children->getContent() == std::string(5000, 'x'),
				"Stream reads must not truncate XML content");
		}
		for (const auto& data : {std::string(), std::string("<Root><Broken></Root>")})
		{
			ChunkStream stream(data, 0, 7);
			Document doc;
			require(
				!doc.open(&stream) && doc.getLastError().find("<stream>") != std::string::npos,
				"Empty and malformed IDataStreams must fail with stream-specific diagnostics");
		}
		ChunkStream stalled("<Root/>", 0, 0);
		Document doc;
		require(!doc.open(&stalled), "A zero-byte read before EOF must terminate rather than loop forever");
	}

	void testErrorResetAndEmptyRoot()
	{
		Document doc;
		require(doc.getLastError().empty(), "A new document must not report an internal parser error");
		require(doc.getRoot() == nullptr, "A new document must expose a null root");
		std::istringstream broken("<Root><Child></Root>");
		require(
			!doc.open(broken) && doc.getLastError().find("<stream>") != std::string::npos,
			"Malformed stream diagnostics must name the stream");
		doc.clearLastError();
		require(doc.getLastError().empty(), "Clearing the last error must remove parser diagnostics");
		std::istringstream good("<Root/>");
		require(doc.open(good) && doc.getLastError().empty(), "A valid load after failure must clear the error");
		doc.clear();
		require(
			doc.getRoot() == nullptr && doc.getLastError().empty(),
			"Clearing must remove the root and reset all errors");
	}

	class FailedOutput : public std::streambuf
	{
		std::streamsize xsputn(const char*, std::streamsize) override
		{
			return 0;
		}
		int_type overflow(int_type) override
		{
			return traits_type::eof();
		}
	};

	void testOutputFailure()
	{
		Document doc;
		doc.createRoot("Root");
		FailedOutput buffer;
		std::ostream output(&buffer);
		require(!doc.save(output), "A failed output stream must not report a successful save");
		require(
			doc.getLastError().find("<stream>") != std::string::npos,
			"Save failures must identify the failed stream");
		std::ostringstream good;
		require(doc.save(good) && doc.getLastError().empty(), "A successful retry must clear the save error");
		std::istringstream input(good.str());
		Document restored;
		require(
			restored.open(input) && restored.getRoot()->getName() == "Root",
			"Failed saves must preserve the document for retry");
	}

	void testFilesAndDiagnostics()
	{
		const auto directory = std::filesystem::temp_directory_path() /
			("mygui-xml-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
		require(std::filesystem::create_directory(directory), "Must create an isolated test directory");
		struct Cleanup
		{
			std::filesystem::path path;
			~Cleanup()
			{
				std::error_code error;
				std::filesystem::remove_all(path, error);
			}
		} cleanup{directory};
		const auto file = directory / "broken.xml";
		{
			std::ofstream output(file);
			output << "<Root>\n  <Child/>\n</Other>\n";
		}
		Document doc;
		require(!doc.open(file), "Mismatched closing tags in files must fail");
		const auto error = doc.getLastError();
		require(
			error.find("broken.xml(3,") != std::string::npos && error.find("</Other>") != std::string::npos,
			"File diagnostics must include the filename, failing line number and source excerpt");
		require(
			!doc.open(directory / "missing.xml") && doc.getLastError().find("missing.xml") != std::string::npos,
			"Missing-file diagnostics must retain the attempted filename");
		doc.clear();
		doc.createRoot("Saved")->createChild("Value", "\xD0\x91\xF0\x9F\x98\x80");
		require(!doc.save(directory / "missing-directory" / "output.xml"), "Saving into a missing directory must fail");
		require(
			doc.getLastError().find("Failed to save") != std::string::npos,
			"Save diagnostics must describe the save failure");
		const MyGUI::UString unicode(
			MyGUI::utility::toUtf8(directory / MyGUI::utility::toPath("\xD0\x91\xF0\x9F\x98\x80.xml")));
		require(
			doc.save(unicode) && doc.getLastError().empty(),
			"A valid Unicode-path save must recover after failure");
		require(
			doc.open(unicode) && doc.getRoot()->getName() == "Saved",
			"UString paths must round-trip through file I/O");
	}

}

int main()
{
	return unittest::runTests({
		{"Element mutation and filtered enumeration", testTreeMutation},
		{"Deep-copy lifetime, declarations and serialization", testCopyLifetimeAndRoundTrip},
		{"Partial and unknown-size data streams", testDataStreams},
		{"Error reset and empty documents", testErrorResetAndEmptyRoot},
		{"Output failure and retry", testOutputFailure},
		{"File diagnostics and Unicode paths", testFilesAndDiagnostics},
	});
}
