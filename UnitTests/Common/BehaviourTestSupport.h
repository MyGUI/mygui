#ifndef MYGUI_UNITTEST_BEHAVIOUR_TEST_SUPPORT_H_
#define MYGUI_UNITTEST_BEHAVIOUR_TEST_SUPPORT_H_

#include "TestSupport.h"
#include <initializer_list>
#include <iostream>

namespace unittest
{

	struct TestCase
	{
		const char* name;
		void (*run)();
	};

	// Report every independent scenario, even if an earlier assertion failed.
	template<typename Tests>
	int runTests(const Tests& _tests)
	{
		int passed = 0;
		int failures = 0;
		for (const auto& test : _tests)
		{
			try
			{
				test.run();
				++passed;
				std::cout << "PASS: " << test.name << '\n';
			}
			catch (const std::exception& error)
			{
				++failures;
				std::cerr << "FAIL: " << test.name << ": " << error.what() << '\n';
			}
		}
		std::cout << passed << " passed, " << failures << " failed\n";
		return failures == 0 && passed != 0 ? 0 : 1;
	}

	inline int runTests(std::initializer_list<TestCase> _tests)
	{
		return runTests<std::initializer_list<TestCase>>(_tests);
	}

	inline void keyStroke(MyGUI::KeyCode _key, MyGUI::Char _text = 0)
	{
		auto& input = MyGUI::InputManager::getInstance();
		input.injectKeyPress(_key, _text);
		input.injectKeyRelease(_key);
	}

	inline void shortcut(MyGUI::KeyCode _key)
	{
		auto& input = MyGUI::InputManager::getInstance();
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		const auto modifier = MyGUI::KeyCode::LeftWindows;
#else
		const auto modifier = MyGUI::KeyCode::LeftControl;
#endif
		input.injectKeyPress(modifier);
		keyStroke(_key);
		input.injectKeyRelease(modifier);
	}

	inline void redo()
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		auto& input = MyGUI::InputManager::getInstance();
		input.injectKeyPress(MyGUI::KeyCode::LeftShift);
		shortcut(MyGUI::KeyCode::Z);
		input.injectKeyRelease(MyGUI::KeyCode::LeftShift);
#else
		shortcut(MyGUI::KeyCode::Y);
#endif
	}

	inline void createInputLayer()
	{
		auto* layer = MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		layer->castType<MyGUI::OverlappedLayer>()->setPick(true);
	}

}

#endif // MYGUI_UNITTEST_BEHAVIOUR_TEST_SUPPORT_H_
