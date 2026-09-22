#include "TestSupport.h"
#include "TestRunner.h"
#include "FixedFont.h"
#include "FileSystemInfo.h"
#include <sstream>
#include <chrono>
#include <fstream>
#include "MyGUI_FileSystemUtility.h"

namespace
{

	using unittest::require;

	void testLayoutInstances()
	{
		unittest::TestContext context;
		auto& gui = context.getGui();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		unittest::loadResources("UnitTest_Resources/TestResources.xml");
		auto& layouts = MyGUI::LayoutManager::getInstance();
		auto* resource = layouts.getByName("BehaviourLayout");
		auto first = resource->createLayout("First_");
		auto second = resource->createLayout("Second_");
		require(
			first.size() == 1 && second.size() == 1 && first[0] != second[0],
			"Each layout instance must own a distinct root");
		auto* button = gui.findWidget<MyGUI::Button>("First_Button");
		auto* leaf = gui.findWidget<MyGUI::Widget>("First_Leaf");
		require(
			first[0]->getName() == "First_Root" && button->getParent() == first[0] && leaf->getParent() == button,
			"Prefixes and hierarchy must apply to all nested widgets");
		require(button->getCoord() == MyGUI::IntCoord(5, 6, 70, 20), "Layout coordinates must be parent-relative");
		require(
			leaf->getAbsolutePosition() == MyGUI::IntPoint(16, 28),
			"Nested layout coordinates must accumulate through parents");
		require(
			button->getStateSelected() && !button->getNeedKeyFocus() && !leaf->getVisible(),
			"Typed properties must be applied while creating widgets");
		require(first[0]->getUserString("purpose") == "layout test", "Layout user strings must be retained");
		button->setStateSelected(false);
		require(
			gui.findWidget<MyGUI::Button>("Second_Button")->getStateSelected(),
			"Layout instances must not share mutable widget state");
		layouts.unloadLayout(first);
		require(
			gui.findWidget<MyGUI::Widget>("First_Root", false) == nullptr &&
				gui.findWidget<MyGUI::Widget>("First_Leaf", false) == nullptr,
			"Unloading must remove the complete instance from widget lookup");
		require(
			gui.findWidget<MyGUI::Widget>("Second_Root", false) == second[0],
			"Unloading one instance must preserve another");
		first = resource->createLayout("First_");
		require(
			gui.findWidget<MyGUI::Button>("First_Button")->getStateSelected(),
			"Recreating a layout must restore its declared defaults");
		layouts.unloadLayout(first);
		layouts.unloadLayout(second);
	}

	void testParentedLayout()
	{
		unittest::TestContext context;
		auto& gui = context.getGui();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		unittest::loadResources("UnitTest_Resources/TestResources.xml");
		auto* parent = gui.createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 200, 400, 300),
			MyGUI::Align::Default,
			"Main");
		auto& layouts = MyGUI::LayoutManager::getInstance();
		auto widgets = layouts.getByName("BehaviourLayout")->createLayout("Child_", parent);
		require(
			widgets.size() == 1 && widgets[0]->getParent() == parent,
			"An explicit layout parent must own the created root");
		require(
			widgets[0]->getAbsolutePosition() == MyGUI::IntPoint(110, 220),
			"Parented layout coordinates must include the parent's origin");
		layouts.unloadLayout(widgets);
		require(
			parent->getChildCount() == 0,
			"Unloading a parented layout must leave its external parent alive and empty");
	}

	void testResourceRemovalAndMissing()
	{
		unittest::TestContext context;
		auto& resources = MyGUI::ResourceManager::getInstance();
		const auto initial = resources.getCount();
		require(
			resources.getByName("AbsentResource", false) == nullptr,
			"Nonthrowing lookup of a missing resource must return nullptr");
		require(!resources.removeByName("AbsentResource"), "Removing a missing resource must report failure");
		bool threw = false;
		try
		{
			resources.getByName("AbsentResource");
		}
		catch (const MyGUI::Exception&)
		{
			threw = true;
		}
		require(threw, "Throwing lookup must report a missing resource");
		for (int iteration = 0; iteration < 3; ++iteration)
		{
			unittest::loadResources("UnitTest_Resources/TestResources.xml");
			require(
				resources.isExist("BehaviourLayout") && resources.getCount() == initial + 1,
				"Loading must register exactly one layout resource");
			require(resources.removeByName("BehaviourLayout"), "Removing a registered resource must succeed");
			require(
				!resources.isExist("BehaviourLayout") && resources.getCount() == initial,
				"Removing resources must restore the registry count");
		}
	}

	void testFontFallback()
	{
		unittest::TestContext context;
		auto& fonts = MyGUI::FontManager::getInstance();
		fonts.setDefaultFont("AbsentDefault");
		require(fonts.getByName("AbsentFont") == nullptr, "An absent font and absent default must return nullptr");
		unittest::registerFixedFont();
		auto* font = fonts.getByName("UnitTestFixedFont");
		require(font != nullptr, "An explicitly registered font must be found");
		for (const char* name : {"", "Default", "AbsentFont"})
			require(
				fonts.getByName(name) == font,
				"Empty, default, and missing names must resolve to the configured default font");
	}

	void testUnicodeLanguageTags()
	{
		unittest::TestContext context;
		auto& language = MyGUI::LanguageManager::getInstance();
		language.addUserTag("face", MyGUI::UString(0x1F600));
		language.addUserTag("letter", MyGUI::UString(0x1D800));
		language.addUserTag("pair", "#{face}#{letter}");
		require(
			language.replaceTags("#{face}").asUTF32() == U"\U0001F600",
			"A supplementary replacement at the end must not advance past the text");
		require(
			language.replaceTags("#{face}#{letter}!").asUTF32() == U"\U0001F600\U0001D800!",
			"Adjacent tags must survive supplementary replacements");
		require(
			language.replaceTags("#{pair}").asUTF32() == U"\U0001F600\U0001D800",
			"Nested replacement passes must use code-point positions");
	}

	void testUnicodeFilePaths()
	{
		require(
			MyGUI::utility::toPath(std::string_view()).empty() &&
				MyGUI::utility::toUtf8(std::filesystem::path()).empty(),
			"Filesystem UTF-8 adapters must preserve empty paths");
		const auto directory = std::filesystem::temp_directory_path() /
			MyGUI::utility::toPath("mygui-\xD0\x91\xF0\x9F\x98\x80-" +
								   std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
		std::filesystem::create_directory(directory);
		struct Cleanup
		{
			std::filesystem::path path;
			~Cleanup()
			{
				std::error_code error;
				std::filesystem::remove_all(path, error);
			}
		} cleanup{directory};
		const MyGUI::UString name(std::u32string(U"\u0411\U0001F600.xml"));
		const auto filename = MyGUI::utility::toPath(name);
		const auto path = directory / filename;
		const auto utf8Path = MyGUI::utility::toUtf8(path);
		MyGUI::xml::Document document;
		std::istringstream input("<MyGUI/>");
		require(document.open(input), "The test document must parse");
		require(document.save(path), "XML must save to a native filesystem path");
		require(document.open(path), "XML must open a native filesystem path");
		require(document.open(utf8Path), "The same Unicode file must open via a UTF-8 path");
		require(document.save(utf8Path), "XML must save to a UTF-8 Unicode path");
		const MyGUI::UString mask(std::u32string(U"\u0411?.xml"));
		require(common::matchWildcard(mask, name), "A wildcard must match one complete supplementary character");
		require(!common::matchWildcard("?.xml", name), "A wildcard must not consume two code points");
		const auto files = common::getSystemFileList(directory, MyGUI::utility::toPath(mask));
		require(
			files.size() == 2 && files[1].name == filename && !files[1].folder,
			"File dialogs must list and filter Unicode filenames");
		common::VectorPath scanned;
		common::scanFolder(scanned, directory, true, mask, true);
		require(scanned.size() == 1 && scanned.front() == path, "Resource scanning must preserve Unicode paths");
		scanned.clear();
		common::scanFolder(scanned, directory, false, mask, false);
		require(
			scanned.size() == 1 && scanned.front() == filename,
			"Filename-only scans must return relative native paths");
		const auto subdirectory = MyGUI::utility::toPath("sub-\xD0\x91\xF0\x9F\x98\x80");
		std::filesystem::create_directory(directory / subdirectory);
		require(document.save(directory / subdirectory / filename), "XML must save inside a Unicode subdirectory");
		scanned.clear();
		common::scanFolder(scanned, directory, true, mask, true);
		require(
			scanned.size() == 2 &&
				std::find(scanned.begin(), scanned.end(), directory / subdirectory / filename) != scanned.end(),
			"Recursive scans must preserve Unicode subdirectory paths");
		const auto nestedFiles = common::getSystemFileList(directory, subdirectory / MyGUI::utility::toPath(mask));
		require(
			nestedFiles.size() == 2 && nestedFiles[1].name == filename,
			"File masks must support a directory prefix");

		std::ifstream file(path, std::ios::binary);
		MyGUI::xml::Document streamed;
		require(file.is_open() && streamed.open(file), "Native paths must open Unicode filenames through file streams");
	}

	void testXmlFailureAndRecovery()
	{
		MyGUI::xml::Document document;
		std::istringstream broken("<MyGUI><Widget></MyGUI>");
		require(!document.open(broken), "Malformed XML must fail to load");
		require(!document.getLastError().empty(), "Malformed XML must provide an error description");
		std::istringstream valid("<MyGUI><Property key=\"a&amp;b\" value=\"&lt;text&gt;\"/></MyGUI>");
		require(document.open(valid), "A document must be reusable after a parse error");
		auto children = document.getRoot()->getElementEnumerator();
		require(children.next("Property"), "A valid document must expose its child element");
		require(
			children->findAttribute("key") == "a&b" && children->findAttribute("value") == "<text>",
			"XML entities must be decoded in attribute values");
		std::ostringstream saved;
		require(document.save(saved), "A parsed document must serialize successfully");
		std::istringstream roundTrip(saved.str());
		MyGUI::xml::Document restored;
		require(restored.open(roundTrip), "Serialized XML must remain valid XML");
		auto restoredChildren = restored.getRoot()->getElementEnumerator();
		require(restoredChildren.next("Property"), "Serialization must preserve child elements");
		require(
			restoredChildren->findAttribute("key") == "a&b" && restoredChildren->findAttribute("value") == "<text>",
			"Serialization must preserve decoded attribute values");
	}

}

int main()
{
	return unittest::runTests({
		{"Unicode language tags", testUnicodeLanguageTags},
		{"Layout instances, properties, and prefixes", testLayoutInstances},
		{"Parented layout ownership", testParentedLayout},
		{"Resource removal and missing lookup", testResourceRemovalAndMissing},
		{"Default font fallback", testFontFallback},
		{"XML failure and recovery", testXmlFailureAndRecovery},
		{"Unicode filenames and wildcards", testUnicodeFilePaths},
	});
}
