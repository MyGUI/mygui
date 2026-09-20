#include "TestSupport.h"
#include "MyGUI_FileSystemUtility.h"
#include <stdexcept>

namespace unittest
{

	void require(bool _condition, std::string_view _message)
	{
		if (!_condition)
			throw std::runtime_error(std::string(_message));
	}

	TestContext::TestContext()
	{
		mPlatform.initialise("");
		mPlatform.getRenderManagerPtr()->setViewSize(800, 600);
		mGui.initialise("");
	}

	TestContext::~TestContext()
	{
		mGui.shutdown();
		mPlatform.shutdown();
	}

	MyGUI::Gui& TestContext::getGui()
	{
		return mGui;
	}

	const MyGUI::IntSize& CountingLayer::getSize() const
	{
		++sizeQueryCount;
		return MyGUI::OverlappedLayer::getSize();
	}

	CountingLayer* createCountingLayer(std::string_view _name, size_t _index)
	{
		MyGUI::FactoryManager::getInstance().registerFactory<CountingLayer>(
			MyGUI::LayerManager::getInstance().getCategoryName());
		return static_cast<CountingLayer*>(
			MyGUI::LayerManager::getInstance().createLayerAt(_name, "CountingLayer", _index));
	}

	std::filesystem::path getResourcePath(std::string_view _filename)
	{
		return MyGUI::utility::toPath(MYGUI_UNITTEST_RESOURCE_ROOT) / MyGUI::utility::toPath(_filename);
	}

	void loadResources(std::string_view _filename)
	{
		const auto filename = getResourcePath(_filename);
		MyGUI::xml::Document document;
		require(document.open(filename), "Failed to load test resources: " + MyGUI::utility::toUtf8(filename));
		MyGUI::ResourceManager::getInstance().loadFromXmlNode(document.getRoot(), "", MyGUI::Version(1, 1));
	}

}
