#include "TestSupport.h"
#include "MyGUI_FileSystemUtility.h"
#include <atomic>
#include <chrono>
#include <fstream>
#include <stdexcept>

namespace unittest
{

	void require(bool _condition, std::string_view _message)
	{
		if (!_condition)
			throw std::runtime_error(std::string(_message));
	}

	TemporaryDirectory::TemporaryDirectory(std::string_view _prefix)
	{
		static std::atomic<unsigned long long> sequence{0};
		const auto root = std::filesystem::temp_directory_path();
		const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
		for (int attempt = 0; attempt < 100; ++attempt)
		{
			mPath = root /
				MyGUI::utility::toPath(
						std::string(_prefix) + std::to_string(stamp) + "-" + std::to_string(sequence.fetch_add(1)));
			std::error_code error;
			if (std::filesystem::create_directory(mPath, error))
				return;
			if (error && error != std::errc::file_exists)
				throw std::filesystem::filesystem_error("Cannot create temporary test directory", mPath, error);
		}
		throw std::runtime_error("Cannot create a unique temporary test directory");
	}

	TemporaryDirectory::~TemporaryDirectory()
	{
		std::error_code error;
		std::filesystem::remove_all(mPath, error);
	}

	const std::filesystem::path& TemporaryDirectory::path() const
	{
		return mPath;
	}

	TemporaryFile::TemporaryFile(std::string_view _content) :
		mPath(mDirectory.path() / "data.bin")
	{
		std::ofstream output(mPath, std::ios::binary);
		if (!_content.empty())
			output.write(_content.data(), static_cast<std::streamsize>(_content.size()));
		output.close();
		require(output.good(), "The temporary file must be writable");
	}

	const std::filesystem::path& TemporaryFile::path() const
	{
		return mPath;
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

	std::filesystem::path getMediaPath(std::string_view _filename)
	{
		return MyGUI::utility::toPath(MYGUI_UNITTEST_MEDIA_ROOT) / MyGUI::utility::toPath(_filename);
	}

	void loadResources(std::string_view _filename)
	{
		const auto filename = getResourcePath(_filename);
		MyGUI::xml::Document document;
		require(document.open(filename), "Failed to load test resources: " + MyGUI::utility::toUtf8(filename));
		MyGUI::ResourceManager::getInstance().loadFromXmlNode(document.getRoot(), "", MyGUI::Version(1, 1));
	}

}
