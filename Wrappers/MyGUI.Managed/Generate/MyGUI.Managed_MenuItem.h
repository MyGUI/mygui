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

		public ref class MenuItem : public Widget
		{

		private:
			typedef MyGUI::MenuItem ThisType;

		public:
			MenuItem() : Widget() { }

		internal:
			MenuItem( MyGUI::MenuItem* _native ) : Widget(_native) { }
			MenuItem( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MenuItem(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		Convert<MyGUI::MenuCtrl *>::Type GetItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::MenuCtrl *>::To(
				static_cast<ThisType*>(mNative)->getItemChild( ) );
		}



   	public:
		property Convert<const MyGUI::UString &>::Type ItemName
		{
			Convert<const MyGUI::UString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::UString &>::To( static_cast<ThisType*>(mNative)->getItemName() );
			}
			void set(Convert<const MyGUI::UString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemName( Convert<const MyGUI::UString &>::From(_value) );
			}
		}
	


   


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
