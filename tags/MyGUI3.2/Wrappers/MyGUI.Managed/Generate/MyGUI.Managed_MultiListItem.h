/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class MultiListItem : public Widget
		{

		private:
			typedef MyGUI::MultiListItem ThisType;

		public:
			MultiListItem() : Widget() { }

		internal:
			MultiListItem( MyGUI::MultiListItem* _native ) : Widget(_native) { }
			MultiListItem( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MultiListItem(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		void SetItemWidth(
			Convert<int>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemWidth(
				Convert<int>::From(_value) );
		}



   	public:
		void SetItemResizingPolicy(
			Convert<MyGUI::ResizingPolicy>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemResizingPolicy(
				Convert<MyGUI::ResizingPolicy>::From(_value) );
		}



   


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
