/*!
	@file
	@author		Albert Semenov
	@date		09/2009
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
#include "InputManager.h"

namespace input
{

	InputManager::InputManager() :
		mInputManager(0),
		mKeyboard(0),
		mMouse(0)
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::createInput(size_t _handle) // создаем систему ввода
	{
		std::ostringstream windowHndStr;
		windowHndStr << _handle;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);

		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		mMouse->setEventCallback(this);
	}

	void InputManager::destroyInput()
	{
		if (mInputManager)
		{
			if (mMouse)
			{
				mInputManager->destroyInputObject( mMouse );
				mMouse = nullptr;
			}
			if (mKeyboard)
			{
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = nullptr;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}

	bool InputManager::mouseMoved(const OIS::MouseEvent& _arg)
	{
		injectMouseMove(_arg.state.X.abs, _arg.state.Y.abs, _arg.state.Z.abs);
		return true;
	}

	bool InputManager::mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
	{
		injectMousePress(_arg.state.X.abs, _arg.state.Y.abs, MyGUI::MouseButton::Enum(_id));
		return true;
	}

	bool InputManager::mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
	{
		injectMouseRelease(_arg.state.X.abs, _arg.state.Y.abs, MyGUI::MouseButton::Enum(_id));
		return true;
	}

	bool InputManager::keyPressed(const OIS::KeyEvent& _arg)
	{
		injectKeyPress(MyGUI::KeyCode::Enum(_arg.key), (MyGUI::Char)_arg.text);
		return true;
	}

	bool InputManager::keyReleased(const OIS::KeyEvent& _arg)
	{
		injectKeyRelease(MyGUI::KeyCode::Enum(_arg.key));
		return true;
	}

	void InputManager::captureInput()
	{
		if (mMouse) mMouse->capture();
		mKeyboard->capture();
	}

	void InputManager::setInputViewSize(int _width, int _height)
	{
		if (mMouse)
		{
			const OIS::MouseState &ms = mMouse->getMouseState();
			ms.width = _width;
			ms.height = _height;
		}
	}

} // namespace input
