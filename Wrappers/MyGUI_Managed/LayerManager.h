/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Generate/MyGUI_Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{
		public ref class LayerManager
		{
		private:
			LayerManager() { }

		public:
			static property LayerManager ^ Instance
			{
				LayerManager ^ get( )
				{
					if (mLayerManager == nullptr)
					{
						mLayerManager = MyGUI::LayerManager::getInstancePtr();
						if (mLayerManager == nullptr)
						{
							throw gcnew System::NullReferenceException();
						}
					}
					return m_instance;
				}
			}

		public:
			void UpWidget(Widget ^ _widget)
			{
				mLayerManager->upLayerItem( Convert< MyGUI::Widget* >::From(_widget) );
			}

		public:
			void AttachToLayer(System::String ^  _layer, Widget ^ _widget)
			{
				mLayerManager->attachToLayerNode( string_utility::managed_to_utf8(_layer) , Convert< MyGUI::Widget* >::From(_widget) );
			}

		private:
			static LayerManager ^ m_instance = gcnew LayerManager();
			static MyGUI::LayerManager* mLayerManager = nullptr;
		};
	}
}
