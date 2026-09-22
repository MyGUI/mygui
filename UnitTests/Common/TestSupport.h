#ifndef MYGUI_UNITTEST_TEST_SUPPORT_H_
#define MYGUI_UNITTEST_TEST_SUPPORT_H_

#include "MyGUI.h"
#include "MyGUI_DummyPlatform.h"
#include "MyGUI_OverlappedLayer.h"
#include <filesystem>
#include <string_view>

namespace unittest
{

	void require(bool _condition, std::string_view _message);

	// Unexpected exception types propagate to the test runner.
	template<typename Exception = MyGUI::Exception, typename Action>
	void requireThrows(Action&& _action, std::string_view _message)
	{
		try
		{
			_action();
		}
		catch (const Exception&)
		{
			return;
		}
		require(false, _message);
	}

	// Owns a newly created directory and removes its contents on scope exit.
	class TemporaryDirectory
	{
	public:
		explicit TemporaryDirectory(std::string_view _prefix = "mygui-test-");
		~TemporaryDirectory();
		TemporaryDirectory(const TemporaryDirectory&) = delete;
		TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;

		const std::filesystem::path& path() const;

	private:
		std::filesystem::path mPath;
	};

	class TemporaryFile
	{
	public:
		explicit TemporaryFile(std::string_view _content);
		const std::filesystem::path& path() const;

	private:
		TemporaryDirectory mDirectory;
		std::filesystem::path mPath;
	};

	class TestContext
	{
	public:
		TestContext();
		~TestContext();
		TestContext(const TestContext&) = delete;
		TestContext& operator=(const TestContext&) = delete;

		MyGUI::Gui& getGui();

	private:
		MyGUI::DummyPlatform mPlatform;
		MyGUI::Gui mGui;
	};

	// Counts layer-size queries so popup tests can detect redundant position calculations.
	class CountingLayer : public MyGUI::OverlappedLayer
	{
		MYGUI_RTTI_DERIVED(CountingLayer)

	public:
		const MyGUI::IntSize& getSize() const override;
		mutable int sizeQueryCount{0};
	};

	CountingLayer* createCountingLayer(std::string_view _name, size_t _index);
	// Resolves a file or directory relative to Media/UnitTests.
	std::filesystem::path getResourcePath(std::string_view _filename);
	// Loads an XML resource file relative to Media/UnitTests.
	void loadResources(std::string_view _filename);

}

#endif // MYGUI_UNITTEST_TEST_SUPPORT_H_
