/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Generate/MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{
		public ref class InputManager
		{
		private:
			InputManager() { }

		public:
			static property InputManager^ Instance
			{
				InputManager^ get( )
				{
					if (mInputManager == nullptr)
					{
						mInputManager = MyGUI::InputManager::getInstancePtr();
						if (mInputManager == nullptr)
						{
							throw gcnew System::NullReferenceException();
						}
					}
					return m_instance;
				}
			}

		public:
			property bool KeyFocus
			{
				bool get() { return mInputManager->getKeyFocusWidget() != nullptr; }
			}

		public:
			property bool MouseFocus
			{
				bool get() { return mInputManager->getMouseFocusWidget() != nullptr; }
			}

		public:
			property Widget^ KeyFocusWidget
			{
				Widget^ get() { return Convert< MyGUI::Widget * >::To(mInputManager->getKeyFocusWidget()); }
				void set(Widget^ _widget) { mInputManager->setKeyFocusWidget( Convert< MyGUI::Widget * >::From(_widget) ); }
			}

		public:
			property Widget^ MouseFocusWidget
			{
				Widget^ get() { return Convert< MyGUI::Widget * >::To(mInputManager->getMouseFocusWidget()); }
			}

		public:
			void ResetKeyFocus()
			{
				mInputManager->setKeyFocusWidget(nullptr);
			}

		public:
			void AddWidgetModal(Widget^ _widget)
			{
				mInputManager->addWidgetModal( Convert< MyGUI::Widget * >::From(_widget) );
			}

		public:
			void RemoveWidgetModal(Widget^ _widget)
			{
				mInputManager->removeWidgetModal( Convert< MyGUI::Widget * >::From(_widget) );
			}

		private:
			static InputManager^ m_instance = gcnew InputManager();
			static MyGUI::InputManager* mInputManager = nullptr;
		};
	}
}
