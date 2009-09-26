/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include <windows.h>
#endif

namespace MyGUI
{
	const unsigned long INPUT_TIME_DOUBLE_CLICK = 250; //measured in milliseconds
	const float INPUT_DELAY_FIRST_KEY = 0.4f;
	const float INPUT_INTERVAL_KEY = 0.05f;

	MYGUI_INSTANCE_IMPLEMENT(InputManager);

	void InputManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mWidgetMouseFocus = 0;
		mWidgetKeyFocus = 0;
		mIsWidgetMouseCapture = false;
		mIsShiftPressed = false;
		mIsControlPressed = false;
		mHoldKey = KeyCode::None;
		mHoldChar = 0;
		mFirstPressKey = true;
		mTimerKey = 0.0f;
		mOldAbsZ = 0;

		m_showHelpers = false;
		m_mouseHelper = nullptr;
		m_keyHelper = nullptr;

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().eventFrameStart += newDelegate(this, &InputManager::frameEntered);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void InputManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().eventFrameStart -= newDelegate(this, &InputManager::frameEntered);
		WidgetManager::getInstance().unregisterUnlinker(this);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool InputManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		// запоминаем позицию
		mMousePosition.set(_absx, _absy);

		// вычисляем прирост по колеса
		int relz = _absz - mOldAbsZ;
		mOldAbsZ = _absz;

		// двигаем курсор
		PointerManager::getInstance().setPosition(mMousePosition);

		// проверка на скролл
		if (relz != 0)
		{
			bool isFocus = isFocusMouse();
			if (mWidgetMouseFocus != nullptr) mWidgetMouseFocus->onMouseWheel(relz);
			return isFocus;
		}

		if (mIsWidgetMouseCapture)
		{
			if (mWidgetMouseFocus != nullptr) mWidgetMouseFocus->onMouseDrag(_absx, _absy);
			else mIsWidgetMouseCapture = false;
			return true;
		}

		// ищем активное окно
		WidgetPtr item = LayerManager::getInstance().getWidgetFromPoint(_absx, _absy);

		// ничего не изменилось
		if (mWidgetMouseFocus == item)
		{
			bool isFocus = isFocusMouse();
			if (mWidgetMouseFocus != nullptr) mWidgetMouseFocus->onMouseMove(_absx, _absy);
			return isFocus;
		}

		if (item)
		{
			// поднимаемся до рута
			WidgetPtr root = item;
			while (root->getParent()) root = root->getParent();

			// проверяем на модальность
			if (!mVectorModalRootWidget.empty())
			{
				if (root != mVectorModalRootWidget.back())
				{
					item = nullptr;
				}
			}
		}

		// в методе может пропасть наш виджет
		WidgetManager::getInstance().addWidgetToUnlink(item);


		//-------------------------------------------------------------------------------------//
		// новый вид рутового фокуса мыши
		WidgetPtr save_widget = nullptr;

		// спускаемся по новому виджету и устанавливаем рутовый фокус
		WidgetPtr root_focus = item;
		while (root_focus != nullptr)
		{
			if (root_focus->mRootMouseActive)
			{
				save_widget = root_focus;
				break;
			}
			root_focus->mRootMouseActive = true;

			// в методе может пропасть наш виджет
			WidgetManager::getInstance().addWidgetToUnlink(root_focus);
			root_focus->onMouseChangeRootFocus(true);
			WidgetManager::getInstance().removeWidgetFromUnlink(root_focus);

			if (root_focus)
				root_focus = root_focus->getParent();
		};

		// спускаемся по старому виджету и сбрасываем фокус
		root_focus = mWidgetMouseFocus;
		while (root_focus != nullptr)
		{
			if (root_focus == save_widget)
			{
				break;
			}
			root_focus->mRootMouseActive = false;

			// в методе может пропасть наш виджет
			WidgetManager::getInstance().addWidgetToUnlink(root_focus);
			root_focus->onMouseChangeRootFocus(false);
			WidgetManager::getInstance().removeWidgetFromUnlink(root_focus);

			if (root_focus)
				root_focus = root_focus->getParent();
		};
		//-------------------------------------------------------------------------------------//


		// смена фокуса, проверяем на доступность виджета
		if ((mWidgetMouseFocus != nullptr) && (mWidgetMouseFocus->isEnabled()))
		{
			mWidgetMouseFocus->onMouseLostFocus(item);
		}

		WidgetManager::getInstance().removeWidgetFromUnlink(item);


		if ((item != nullptr) && (item->isEnabled()))
		{
			if (item->getPointer() != mPointer)
			{
				mPointer = item->getPointer();
				if (mPointer.empty())
				{
					PointerManager::getInstance().setDefaultPointer();
					eventChangeMousePointer(PointerManager::getInstance().getDefaultPointer());
				}
				else
				{
					PointerManager::getInstance().setPointer(mPointer, item);
					eventChangeMousePointer(mPointer);
				}
			}
			item->onMouseMove(_absx, _absy);
			item->onMouseSetFocus(mWidgetMouseFocus);

		}
		// сбрасываем курсор
		else if (false == mPointer.empty())
		{
			PointerManager::getInstance().setDefaultPointer();
			mPointer.clear();

			eventChangeMousePointer(PointerManager::getInstance().getDefaultPointer());
		}

		// запоминаем текущее окно
		mWidgetMouseFocus = item;

		// обновляем данные
		if (m_showHelpers) updateFocusWidgetHelpers();

		return isFocusMouse();
	}

	bool InputManager::injectMousePress(int _absx, int _absy, MouseButton _id)
	{
		// если мы щелкнули не на гуй
		if (false == isFocusMouse())
		{
			resetKeyFocusWidget();
			return false;
		}

		// если активный элемент заблокирован
		//FIXME
		if (false == mWidgetMouseFocus->isEnabled())
			return true;

		// захватываем только по левой клавише и только если виджету надо
		if (MouseButton::Left == _id)
		{
			// захват окна
			mIsWidgetMouseCapture = true;
			// запоминаем место нажатия
			mLastLeftPressed.set(_absx, _absy);
		}

		// ищем вверх тот виджет который может принимать фокус
		WidgetPtr item = mWidgetMouseFocus;
		while ((item != nullptr) && (false == item->isNeedKeyFocus())) item = item->getParent();

		// устанавливаем перед вызовом т.к. возможно внутри ктонить поменяет фокус под себя
		setKeyFocusWidget(item);

		if (mWidgetMouseFocus != nullptr)
		{
			mWidgetMouseFocus->onMouseButtonPressed(_absx, _absy, _id);

			// после пресса может сброситься
			if (mWidgetMouseFocus)
			{
				// поднимаем виджет, надо подумать что делать если поменялся фокус клавы
				LayerManager::getInstance().upLayerItem(mWidgetMouseFocus);

				// поднимаем пикинг Overlapped окон
				WidgetPtr pick = mWidgetMouseFocus;
				do
				{
					// если оверлаппед, то поднимаем пикинг
					if (pick->getWidgetStyle() == WidgetStyle::Overlapped)
					{
						if (pick->getParent()) pick->getParent()->_forcePeek(pick);
					}

					pick = pick->getParent();
				}
				while (pick);
			}
		}

		return true;
	}

	bool InputManager::injectMouseRelease(int _absx, int _absy, MouseButton _id)
	{

		if (isFocusMouse())
		{
			// если активный элемент заблокирован
			if (false == mWidgetMouseFocus->isEnabled()) return true;

			mWidgetMouseFocus->onMouseButtonReleased(_absx, _absy, _id);

			if (mIsWidgetMouseCapture)
			{
				// сбрасываем захват
				mIsWidgetMouseCapture = false;

				// после вызова, виджет может быть сброшен
				if (nullptr != mWidgetMouseFocus)
				{
					if ((MouseButton::Left == _id) && mTimer.getMilliseconds() < INPUT_TIME_DOUBLE_CLICK)
					{
						mWidgetMouseFocus->onMouseButtonClick();
						// после вызова, виджет может быть сброшен
						if (nullptr != mWidgetMouseFocus) mWidgetMouseFocus->onMouseButtonDoubleClick();
					}
					else
					{
						// проверяем над тем ли мы окном сейчас что и были при нажатии
						WidgetPtr item = LayerManager::getInstance().getWidgetFromPoint(_absx, _absy);
						if ( item == mWidgetMouseFocus)
						{
							mWidgetMouseFocus->onMouseButtonClick();
						}
						mTimer.reset();
					}
				}
			}

			// для корректного отображения
			injectMouseMove(_absx, _absy, mOldAbsZ);

			return true;
		}

		return false;
	}

	bool InputManager::injectKeyPress(KeyCode _key, Char _text)
	{
		// проверка на переключение языков
		firstEncoding(_key, true);

		// запоминаем клавишу
		storeKey(_key, _text);

		bool wasFocusKey = isFocusKey();

		//Pass keystrokes to the current active text widget
		if (isFocusKey())
		{
			mWidgetKeyFocus->onKeyButtonPressed(_key, _text);
		}

		return wasFocusKey;
	}

	bool InputManager::injectKeyRelease(KeyCode _key)
	{
		// проверка на переключение языков
		firstEncoding(_key, false);

		// сбрасываем клавишу
		resetKey();

		bool wasFocusKey = isFocusKey();

		if (isFocusKey()) mWidgetKeyFocus->onKeyButtonReleased(_key);

		return wasFocusKey;
	}

	void InputManager::firstEncoding(KeyCode _key, bool bIsKeyPressed)
	{
		if ((_key == KeyCode::LeftShift) || (_key == KeyCode::RightShift)) mIsShiftPressed = bIsKeyPressed;
		if ((_key == KeyCode::LeftControl) || (_key == KeyCode::RightControl)) mIsControlPressed = bIsKeyPressed;
	}

/*#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

	Char translateWin32Text( KeyCode kc )
	{
		static WCHAR deadKey = 0;

		BYTE keyState[256];
		HKL  layout = GetKeyboardLayout(0);
		if ( GetKeyboardState(keyState) == 0 )
			return 0;

		int code = *((int*)&kc);
		unsigned int vk = MapVirtualKeyEx((UINT)code, 3, layout);
		if ( vk == 0 )
			return 0;

		WCHAR buff[3] = { 0, 0, 0 };
		int ascii = ToUnicodeEx(vk, (UINT)code, keyState, buff, 3, 0, layout);
		if (ascii == 1 && deadKey != '\0' )
		{
			// A dead key is stored and we have just converted a character key
			// Combine the two into a single character
			WCHAR wcBuff[3] = { buff[0], deadKey, '\0' };
			WCHAR out[3];

			deadKey = '\0';
			if(FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
				return out[0];
		}
		else if (ascii == 1)
		{
			// We have a single character
			deadKey = '\0';
			return buff[0];
		}
		else if(ascii == 2)
		{
			// Convert a non-combining diacritical mark into a combining diacritical mark
			// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
			// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
			switch(buff[0])	{
			case 0x5E: // Circumflex accent: в
				deadKey = 0x302; break;
			case 0x60: // Grave accent: а
				deadKey = 0x300; break;
			case 0xA8: // Diaeresis: ь
				deadKey = 0x308; break;
			case 0xB4: // Acute accent: й
				deadKey = 0x301; break;
			case 0xB8: // Cedilla: з
				deadKey = 0x327; break;
			default:
				deadKey = buff[0]; break;
			}
		}

		return 0;
	}

#endif*/

	void InputManager::setKeyFocusWidget(WidgetPtr _widget)
	{
		if (_widget == mWidgetKeyFocus) return;

		//-------------------------------------------------------------------------------------//
		// новый вид рутового фокуса
		WidgetPtr save_widget = nullptr;

		// спускаемся по новому виджету и устанавливаем рутовый фокус
		WidgetPtr root_focus = _widget;
		while (root_focus != nullptr)
		{
			if (root_focus->mRootKeyActive)
			{
				save_widget = root_focus;
				break;
			}
			root_focus->mRootKeyActive = true;

			// в методе может пропасть наш виджет
			WidgetManager::getInstance().addWidgetToUnlink(root_focus);
			root_focus->onKeyChangeRootFocus(true);
			WidgetManager::getInstance().removeWidgetFromUnlink(root_focus);

			if (root_focus)
				root_focus = root_focus->getParent();
		};

		// спускаемся по старому виджету и сбрасываем фокус
		root_focus = mWidgetKeyFocus;
		while (root_focus != nullptr)
		{
			if (root_focus == save_widget)
			{
				break;
			}
			root_focus->mRootKeyActive = false;

			// в методе может пропасть наш виджет
			WidgetManager::getInstance().addWidgetToUnlink(root_focus);
			root_focus->onKeyChangeRootFocus(false);
			WidgetManager::getInstance().removeWidgetFromUnlink(root_focus);

			if (root_focus)
				root_focus = root_focus->getParent();
		};
		//-------------------------------------------------------------------------------------//


		// сбрасываем старый
		if (mWidgetKeyFocus)
		{
			mWidgetKeyFocus->onKeyLostFocus(_widget);
		}

		// устанавливаем новый
		if (_widget && _widget->isNeedKeyFocus())
		{
			_widget->onKeySetFocus(mWidgetKeyFocus);
		}

		mWidgetKeyFocus = _widget;

		// обновляем данные
		if (m_showHelpers) updateFocusWidgetHelpers();
	}

	void InputManager::resetMouseFocusWidget()
	{
		// спускаемся по старому виджету и сбрасываем фокус
		WidgetPtr root_focus = mWidgetMouseFocus;
		while (root_focus != nullptr)
		{
			root_focus->mRootMouseActive = false;

			// в методе может пропасть наш виджет
			WidgetManager::getInstance().addWidgetToUnlink(root_focus);
			root_focus->onMouseChangeRootFocus(false);
			WidgetManager::getInstance().removeWidgetFromUnlink(root_focus);

			if (root_focus)
				root_focus = root_focus->getParent();
		};

		mIsWidgetMouseCapture = false;
		if (nullptr != mWidgetMouseFocus)
		{
			mWidgetMouseFocus->onMouseLostFocus(nullptr);
			mWidgetMouseFocus = nullptr;
		}

		// обновляем данные
		if (m_showHelpers) updateFocusWidgetHelpers();

	}

	// удаляем данный виджет из всех возможных мест
	void InputManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (nullptr == _widget) return;
		if (_widget == mWidgetMouseFocus)
		{
			mIsWidgetMouseCapture = false;
			mWidgetMouseFocus = nullptr;

			// обновляем данные
			if (m_showHelpers) updateFocusWidgetHelpers();
		}
		if (_widget == mWidgetKeyFocus)
		{
			mWidgetKeyFocus = nullptr;

			// обновляем данные
			if (m_showHelpers) updateFocusWidgetHelpers();
		}

		// ручками сбрасываем, чтобы не менять фокусы
		for (VectorWidgetPtr::iterator iter=mVectorModalRootWidget.begin(); iter!=mVectorModalRootWidget.end(); ++iter)
		{
			if ((*iter == _widget))
			{
				mVectorModalRootWidget.erase(iter);
				break;
			}
		}

	}

	void InputManager::addWidgetModal(WidgetPtr _widget)
	{
		if (nullptr == _widget) return;
		MYGUI_ASSERT(nullptr == _widget->getParent(), "Modal widget must be root");

		resetMouseFocusWidget();
		removeWidgetModal(_widget);
		mVectorModalRootWidget.push_back(_widget);

		setKeyFocusWidget(_widget);
		LayerManager::getInstance().upLayerItem(_widget);
	}

	void InputManager::removeWidgetModal(WidgetPtr _widget)
	{
		resetKeyFocusWidget(_widget);
		resetMouseFocusWidget();

		for (VectorWidgetPtr::iterator iter=mVectorModalRootWidget.begin(); iter!=mVectorModalRootWidget.end(); ++iter)
		{
			if ((*iter == _widget))
			{
				mVectorModalRootWidget.erase(iter);
				break;
			}
		}
		// если еще есть модальные то их фокусируем и поднимаем
		if (false == mVectorModalRootWidget.empty())
		{
			setKeyFocusWidget(mVectorModalRootWidget.back());
			LayerManager::getInstance().upLayerItem(mVectorModalRootWidget.back());
		}
	}

	void InputManager::storeKey(KeyCode _key, Char _text)
	{
		mHoldKey = KeyCode::None;
		mHoldChar = 0;

		if ( false == isFocusKey() ) return;
		if ( (_key == KeyCode::LeftShift) || (_key == KeyCode::RightShift)
			|| (_key == KeyCode::LeftControl) || (_key == KeyCode::RightControl)
			|| (_key == KeyCode::LeftAlt) || (_key == KeyCode::RightAlt)
			) return;

		mFirstPressKey = true;
		mHoldKey = _key;
		mHoldChar = _text;
		mTimerKey = 0.0f;
	}

	void InputManager::resetKey()
	{
		mHoldKey = KeyCode::None;
		mHoldChar = 0;
	}

	void InputManager::frameEntered(float _frame)
	{

		// обновляем данные
		if (m_showHelpers) updateFocusWidgetHelpers();

		if ( mHoldKey == KeyCode::None) return;
		if ( false == isFocusKey() )
		{
			mHoldKey = KeyCode::None;
			mHoldChar = 0;
			return;
		}

		mTimerKey += _frame;

		if (mFirstPressKey)
		{
			if (mTimerKey > INPUT_DELAY_FIRST_KEY)
			{
				mFirstPressKey = false;
				mTimerKey = 0.0f;
			}
		}
		else
		{
			if (mTimerKey > INPUT_INTERVAL_KEY)
			{
				while (mTimerKey > INPUT_INTERVAL_KEY) mTimerKey -= INPUT_INTERVAL_KEY;
				mWidgetKeyFocus->onKeyButtonPressed(mHoldKey, mHoldChar);
				// focus can be dropped in onKeyButtonPressed
				if ( isFocusKey() ) mWidgetKeyFocus->onKeyButtonReleased(mHoldKey);
			}
		}

	}

	void InputManager::setShowFocus(bool _show)
	{
		m_showHelpers = _show;
		if (!m_showHelpers)
		{
			if (m_mouseHelper) m_mouseHelper->setVisible(false);
			if (m_keyHelper) m_keyHelper->setVisible(false);
		}
	}

	void InputManager::updateFocusWidgetHelpers()
	{

		static const std::string layer = "Statistic";
		static const std::string skin_mouse = "RectGreen";
		static const std::string skin_key = "RectBlue";

		static WidgetPtr mouse_focus = nullptr;
		if ((mWidgetMouseFocus != mouse_focus)
			|| ((mWidgetMouseFocus != nullptr)
			&& (m_mouseHelper != nullptr)
			&& mWidgetMouseFocus->getAbsoluteCoord() != m_mouseHelper->getAbsoluteCoord()))
		{
			mouse_focus = mWidgetMouseFocus;

			if (m_mouseHelper == nullptr)
			{
				if (!LayerManager::getInstance().isExist(layer)) return;
				if (!SkinManager::getInstance().isExist(skin_mouse)) return;
				m_mouseHelper = Gui::getInstance().createWidget<Widget>(skin_mouse, IntCoord(), Align::Default, layer);
				m_mouseHelper->setNeedMouseFocus(false);
			}

			if (mWidgetMouseFocus)
			{
				MYGUI_OUT("mouse focus : ", mWidgetMouseFocus->getName());
				m_mouseHelper->setCoord(mWidgetMouseFocus->getAbsoluteCoord());
				m_mouseHelper->setVisible(true);
			}
			else
			{
				MYGUI_OUT("mouse focus : nullptr");
				m_mouseHelper->setVisible(false);
			}
		}

		static WidgetPtr key_focus = nullptr;
		if ((mWidgetKeyFocus != key_focus)
			|| ((mWidgetKeyFocus != nullptr)
			&& (m_keyHelper != nullptr)
			&& mWidgetKeyFocus->getAbsoluteCoord() != m_keyHelper->getAbsoluteCoord()))
		{
			key_focus = mWidgetKeyFocus;

			if (m_keyHelper == nullptr)
			{
				if (!LayerManager::getInstance().isExist(layer)) return;
				if (!SkinManager::getInstance().isExist(skin_key)) return;
				m_keyHelper = Gui::getInstance().createWidget<Widget>(skin_key, IntCoord(), Align::Default, layer);
				m_keyHelper->setNeedMouseFocus(false);
			}
			if (mWidgetKeyFocus)
			{
				MYGUI_OUT("key focus : ", mWidgetKeyFocus->getName());
				m_keyHelper->setCoord(mWidgetKeyFocus->getAbsoluteCoord());
				m_keyHelper->setVisible(true);
			}
			else
			{
				MYGUI_OUT("key focus : nullptr");
				m_keyHelper->setVisible(false);
			}
		}
	}

	void InputManager::resetKeyFocusWidget(WidgetPtr _widget)
	{
		if (mWidgetKeyFocus == _widget)
			setKeyFocusWidget(nullptr);
	}

} // namespace MyGUI
