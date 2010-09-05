/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#pragma once

#include <MyGUI.h>

namespace MyGUI
{
	namespace Managed
	{
		public ref class PointerManager
		{
		private:
			PointerManager() { }

		public:
			static property PointerManager ^ Instance
			{
				PointerManager ^ get( )
				{
					if (mPointerManager == nullptr)
					{
						mPointerManager = MyGUI::PointerManager::getInstancePtr();
						if (mPointerManager == nullptr)
						{
							throw gcnew System::NullReferenceException();
						}
					}
					return m_instance;
				}
			}

		public:
			property bool Visible
			{
				bool get()
				{
					return mPointerManager->isVisible();
				}
				void set(bool _value)
				{
					mPointerManager->setVisible(_value);
				}
			}

		private:
			static PointerManager ^ m_instance = gcnew PointerManager();
			static MyGUI::PointerManager* mPointerManager = nullptr;
		};
	}
}
