/*!
	@file
	@author		Albert Semenov
	@date		05/2009
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "precompiled.h"
#include "BaseManager.h"

#include "GL/glfw.h"

namespace base
{

	void GLFWCALL windowResize( int _width, int _height )
	{
		BaseManager::getInstance()._windowResize(_width, _height);
	}

	void GLFWCALL keyEvent(int _key, int _state)
	{
		if (!glfwGetWindowParam(GLFW_ACTIVE))
			return;

		BaseManager::getInstance()._keyEvent(_key, _state);
	}

	void GLFWCALL mousePosEvent(int _x, int _y)
	{
		if (!glfwGetWindowParam(GLFW_ACTIVE))
			return;

		BaseManager::getInstance()._mousePosEvent(_x, _y);
	}

	void GLFWCALL mouseWheelEvent(int _wheel)
	{
		if (!glfwGetWindowParam(GLFW_ACTIVE))
			return;

		BaseManager::getInstance()._mouseWheelEvent(_wheel);
	}

	void GLFWCALL mouseButtonEvent(int _button, int _state)
	{
		if (!glfwGetWindowParam(GLFW_ACTIVE))
			return;

		BaseManager::getInstance()._mouseButtonEvent(_button, _state);
	}

	BaseManager * BaseManager::m_instance = nullptr;
	BaseManager & BaseManager::getInstance()
	{
		assert(m_instance);
		return *m_instance;
	}

	BaseManager::BaseManager() :
		mWidth(800),
		mHeight(600),
		mQuit(false),
		mGUI(nullptr),
		mPlatform(nullptr),
		mInfo(nullptr),
		mRootMedia("../../Media"),
		mMouseRealX(0),
		mMouseRealY(0),
		mMouseX(0),
		mMouseY(0),
		mMouseWheel(0)
	{
		assert(!m_instance);
		m_instance = this;
	}

	BaseManager::~BaseManager()
	{
		m_instance = nullptr;
	}

	void BaseManager::createInput()
	{
		glfwSetKeyCallback(keyEvent);
		glfwSetMousePosCallback(mousePosEvent);
		glfwSetMouseWheelCallback(mouseWheelEvent);
		glfwSetMouseButtonCallback(mouseButtonEvent);
		glfwDisable(GLFW_MOUSE_CURSOR);
	}

	void BaseManager::destroyInput()
	{
		glfwSetKeyCallback(0);
		glfwSetMousePosCallback(0);
		glfwSetMouseWheelCallback(0);
		glfwSetMouseButtonCallback(0);
		glfwEnable(GLFW_MOUSE_CURSOR);
	}

	bool BaseManager::create()
	{
		if ( !glfwInit() )
		{
			return false;
		}

		GLFWvidmode mode;
		glfwGetDesktopMode(&mode);

		// Open OpenGL window
		if ( !glfwOpenWindow(mWidth, mHeight, 0, 0, 0, 0, 16, 0, GLFW_WINDOW))
		{
			glfwTerminate();
			return false;
		}

		glfwSetWindowPos((mode.Width - mWidth) / 2, (mode.Height - mHeight) / 2);

		glfwSetWindowSizeCallback( windowResize );

		createGui();
		createInput();
		createScene();

		return true;
	}

	void BaseManager::run()
	{
		while (!mQuit && glfwGetWindowParam(GLFW_OPENED))
		{
			clearFrame();
			drawOneFrame();
	        glfwSwapBuffers();
		}
	}

	void BaseManager::destroy()
	{
		destroyScene();
		destroyGui();
		destroyInput();

		if (glfwGetWindowParam(GLFW_OPENED))
			glfwCloseWindow();

	    // Terminate GLFW
		glfwTerminate();
	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::OpenGLPlatform();
		mPlatform->initialise();

		setupResources();

		mPlatform->getRenderManagerPtr()->setViewSize(mWidth, mHeight);

		mGUI = new MyGUI::Gui();
		mGUI->initialise("core_ogltest.xml");

		mInfo = new statistic::StatisticInfo();
	}

	void BaseManager::destroyGui()
	{
		if (mGUI)
		{
			if (mInfo)
			{
				delete mInfo;
				mInfo = nullptr;
			}

			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::setWindowCaption(const std::string & _text)
	{
		glfwSetWindowTitle(_text.c_str());
	}

	void BaseManager::setWallpaper(const std::string & _filename)
	{
		static MyGUI::StaticImagePtr image = nullptr;
		if (image == nullptr) image = mGUI->createWidget<MyGUI::StaticImage>(
			"StaticImage",
			MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()),
			MyGUI::Align::Stretch,
			"Back");
		image->setImageTexture(_filename);
		image->setNeedMouseFocus(false);
	}

	void BaseManager::setDescriptionText(const MyGUI::UString & _text)
	{
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string & _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void BaseManager::setupResources()
	{
		addResourceLocation(mRootMedia, false);
		addResourceLocation(mRootMedia + "/MyGUI_Media", false);
	}	

	void BaseManager::_windowResize(int _width, int _height)
	{
		mWidth = _width;
		mHeight = _height;

		glViewport(0, 0, mWidth, mHeight);

		if (mGUI)
		{
			mPlatform->getRenderManagerPtr()->setViewSize(mWidth, mHeight);

			mGUI->resizeWindow(MyGUI::IntSize(mWidth, mHeight));
			// check position
			_mousePosEvent(mMouseRealX, mMouseRealY);
		}
	}

	void BaseManager::clearFrame()
	{
		glClearColor(0, 0, 0, 0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void BaseManager::drawOneFrame()
	{
		static unsigned long last_time = 0;
		static MyGUI::Timer timer;
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		mGUI->injectFrameEntered((float)((double)(time) / (double)1000));

		last_time = now_time;

		if (mInfo)
		{
			// calc FPS
			const unsigned long interval = 1000; 
			static unsigned long accumulate = 0;
			static int count_frames = 0;
			accumulate += time;
			if (accumulate > interval)
			{
				mInfo->change("FPS", (int)((unsigned long)count_frames * 1000 / accumulate));
				mInfo->update();

				accumulate = 0;
				count_frames = 0;
			}
			count_frames ++;
		}

		mPlatform->getRenderManagerPtr()->drawOneFrame();
	}

	void BaseManager::_keyEvent(int _key, int _state)
	{
		if (_key == GLFW_KEY_ESC)
			quit();

		int scan_code = 0;

		if (_key > GLFW_KEY_SPECIAL)
		{
			int index = _key - GLFW_KEY_SPECIAL - 1;
			if (index >= 0 && index < 64)
			{
				static int special_key[64] =
				{
					MyGUI::KeyCode::Escape,
					MyGUI::KeyCode::F1,
					MyGUI::KeyCode::F2,
					MyGUI::KeyCode::F3,
					MyGUI::KeyCode::F4,
					MyGUI::KeyCode::F5,
					MyGUI::KeyCode::F6,
					MyGUI::KeyCode::F7,
					MyGUI::KeyCode::F8,
					MyGUI::KeyCode::F9,
					MyGUI::KeyCode::F10,
					MyGUI::KeyCode::F11,
					MyGUI::KeyCode::F12,
					MyGUI::KeyCode::F13,
					MyGUI::KeyCode::F14,
					MyGUI::KeyCode::F15,
					0,//MyGUI::KeyCode::F16,
					0,//MyGUI::KeyCode::F17,
					0,//MyGUI::KeyCode::F18,
					0,//MyGUI::KeyCode::F19,
					0,//MyGUI::KeyCode::F20,
					0,//MyGUI::KeyCode::F21,
					0,//MyGUI::KeyCode::F22,
					0,//MyGUI::KeyCode::F23,
					0,//MyGUI::KeyCode::F24,
					0,//MyGUI::KeyCode::F25,
					MyGUI::KeyCode::ArrowUp,
					MyGUI::KeyCode::ArrowDown,
					MyGUI::KeyCode::ArrowLeft,
					MyGUI::KeyCode::ArrowRight,
					MyGUI::KeyCode::LeftShift,
					MyGUI::KeyCode::RightShift,
					MyGUI::KeyCode::LeftControl,
					MyGUI::KeyCode::RightControl,
					MyGUI::KeyCode::LeftAlt,
					MyGUI::KeyCode::RightAlt,
					MyGUI::KeyCode::Tab,
					MyGUI::KeyCode::Return,
					MyGUI::KeyCode::Backspace,
					MyGUI::KeyCode::Insert,
					MyGUI::KeyCode::Delete,
					MyGUI::KeyCode::PageUp,
					MyGUI::KeyCode::PageDown,
					MyGUI::KeyCode::Home,
					MyGUI::KeyCode::End,
					MyGUI::KeyCode::Numpad0,
					MyGUI::KeyCode::Numpad1,
					MyGUI::KeyCode::Numpad2,
					MyGUI::KeyCode::Numpad3,
					MyGUI::KeyCode::Numpad4,
					MyGUI::KeyCode::Numpad5,
					MyGUI::KeyCode::Numpad6,
					MyGUI::KeyCode::Numpad7,
					MyGUI::KeyCode::Numpad8,
					MyGUI::KeyCode::Numpad9,
					MyGUI::KeyCode::Divide,
					MyGUI::KeyCode::Multiply,
					MyGUI::KeyCode::Subtract,
					MyGUI::KeyCode::Add,
					MyGUI::KeyCode::Decimal,
					MyGUI::KeyCode::NumpadEquals,
					MyGUI::KeyCode::NumpadEnter,
					0,
					0,
				};

				scan_code = special_key[index];
			}
		}
		else if (_key == GLFW_KEY_SPACE)
		{
			scan_code = MyGUI::KeyCode::Space;
		}
		else
		{
			const int min_key = 33;
			const int max_key = 96;
			const int count_key = max_key - min_key + 1;
			if (count_key == 64 && _key >= min_key && _key <= max_key)
			{
				static int symbol_key[count_key] =
				{
					0,
					0,
					0,
					0,
					0,
					0,
					MyGUI::KeyCode::Apostrophe,
					0,
					0,
					0,

					0,
					MyGUI::KeyCode::Comma,
					MyGUI::KeyCode::Minus,
					MyGUI::KeyCode::Period,
					MyGUI::KeyCode::Slash,
					MyGUI::KeyCode::Zero,
					MyGUI::KeyCode::One,
					MyGUI::KeyCode::Two,
					MyGUI::KeyCode::Three,
					MyGUI::KeyCode::Four,

					MyGUI::KeyCode::Five,
					MyGUI::KeyCode::Six,
					MyGUI::KeyCode::Seven,
					MyGUI::KeyCode::Eight,
					MyGUI::KeyCode::Nine,
					0,
					MyGUI::KeyCode::Semicolon,
					0,
					MyGUI::KeyCode::Equals,
					0,

					0,
					0,
					MyGUI::KeyCode::A,
					MyGUI::KeyCode::B,
					MyGUI::KeyCode::C,
					MyGUI::KeyCode::D,
					MyGUI::KeyCode::E,
					MyGUI::KeyCode::F,
					MyGUI::KeyCode::G,
					MyGUI::KeyCode::H,
					MyGUI::KeyCode::I,
					MyGUI::KeyCode::J,
					MyGUI::KeyCode::K,
					MyGUI::KeyCode::L,
					MyGUI::KeyCode::M,
					MyGUI::KeyCode::N,
					MyGUI::KeyCode::O,
					MyGUI::KeyCode::P,
					MyGUI::KeyCode::Q,
					MyGUI::KeyCode::R,
					MyGUI::KeyCode::S,
					MyGUI::KeyCode::T,
					MyGUI::KeyCode::U,
					MyGUI::KeyCode::V,
					MyGUI::KeyCode::W,
					MyGUI::KeyCode::X,
					MyGUI::KeyCode::Y,
					MyGUI::KeyCode::Z,
					MyGUI::KeyCode::LeftBracket,
					MyGUI::KeyCode::Backslash,
					MyGUI::KeyCode::RightBracket,
					0,
					0,
					MyGUI::KeyCode::Grave,
				};

				int index = _key - min_key;
				scan_code = symbol_key[index];
			}

			/*static int max_key = 0;
			static int min_key = 100;

			if (_key == 32)
			{
			}
			else
			{
				if (_key < min_key)
					min_key = _key;
				if (_key > max_key)
					max_key = _key;
			}*/

			MyGUI::MYGUI_OUT(MyGUI::utility::toString(_key/* - min_key*/));
		}

		if (_state == GLFW_PRESS)
			mGUI->injectKeyPress(MyGUI::KeyCode::Enum(scan_code));
		else
			mGUI->injectKeyRelease(MyGUI::KeyCode::Enum(scan_code));
		
	}

	void BaseManager::_mousePosEvent(int _x, int _y)
	{
		int diff_x = _x - mMouseRealX;
		int diff_y = _y - mMouseRealY;

		mMouseRealX = _x;
		mMouseRealY = _y;

		mMouseX += diff_x;
		mMouseY += diff_y;

		if (mMouseX < 0) mMouseX = 0;
		else if (mMouseX >= mWidth) mMouseX = mWidth - 1;
		if (mMouseY < 0) mMouseY = 0;
		else if (mMouseY >= mHeight) mMouseY = mHeight - 1;

		mGUI->injectMouseMove(mMouseX, mMouseY, mMouseWheel);
	}

	void BaseManager::_mouseWheelEvent(int _wheel)
	{
		mMouseWheel = _wheel;
		mGUI->injectMouseMove(mMouseX, mMouseY, mMouseWheel);
	}

	void BaseManager::_mouseButtonEvent(int _button, int _state)
	{
		if (_state == GLFW_PRESS)
			mGUI->injectMousePress(mMouseX, mMouseY, MyGUI::MouseButton::Enum(_button));
		else
			mGUI->injectMouseRelease(mMouseX, mMouseY, MyGUI::MouseButton::Enum(_button));
	}

} // namespace base
