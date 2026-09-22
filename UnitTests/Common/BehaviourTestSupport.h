#ifndef MYGUI_UNITTEST_BEHAVIOUR_TEST_SUPPORT_H_
#define MYGUI_UNITTEST_BEHAVIOUR_TEST_SUPPORT_H_

#include "TestSupport.h"

namespace unittest
{

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

	inline void clickAt(int _x, int _y, int _wheel = 0)
	{
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(_x, _y, _wheel);
		input.injectMousePress(_x, _y, MyGUI::MouseButton::Left);
		input.injectMouseRelease(_x, _y, MyGUI::MouseButton::Left);
	}

	inline void dragFromTo(MyGUI::IntPoint _from, MyGUI::IntPoint _to)
	{
		auto& input = MyGUI::InputManager::getInstance();
		input.injectMouseMove(_from.left, _from.top, 0);
		input.injectMousePress(_from.left, _from.top, MyGUI::MouseButton::Left);
		input.injectMouseMove(_to.left, _to.top, 0);
		input.injectMouseRelease(_to.left, _to.top, MyGUI::MouseButton::Left);
	}

}

#endif // MYGUI_UNITTEST_BEHAVIOUR_TEST_SUPPORT_H_
