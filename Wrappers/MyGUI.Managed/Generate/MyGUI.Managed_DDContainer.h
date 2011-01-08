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

		public ref class DDContainer : public Widget
		{

		private:
			typedef MyGUI::DDContainer ThisType;

		public:
			DDContainer() : Widget() { }

		internal:
			DDContainer( MyGUI::DDContainer* _native ) : Widget(_native) { }
			DDContainer( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew DDContainer(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint





   	public:
		delegate void HandleDragWidgetInfo(
			Convert<MyGUI::DDContainer *>::Type _sender ,
			Convert<MyGUI::Widget*&>::Type _item ,
			Convert<MyGUI::types::TCoord< int > &>::Type _dimension );
		event HandleDragWidgetInfo^ RequestDragWidgetInfo
		{
			void add(HandleDragWidgetInfo^ _value)
			{
				mDelegateDragWidgetInfo += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestDragWidgetInfo =
					static_cast< MyGUI::delegates::IDelegate3<
						MyGUI::DDContainer * ,
						MyGUI::Widget*& ,
						MyGUI::types::TCoord< int > & > *>(
							new Delegate3_Ref2< HandleDragWidgetInfo^ ,
							MyGUI::DDContainer * ,
							MyGUI::Widget*& ,
							MyGUI::types::TCoord< int > & >(mDelegateDragWidgetInfo) );
			}
			void remove(HandleDragWidgetInfo^ _value)
			{
				mDelegateDragWidgetInfo -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDragWidgetInfo == nullptr)
					static_cast<ThisType*>(mNative)->requestDragWidgetInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestDragWidgetInfo =
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::DDContainer * ,
							MyGUI::Widget*& ,
							MyGUI::types::TCoord< int > & > *>(
								new Delegate3_Ref2< HandleDragWidgetInfo^ ,
									MyGUI::DDContainer * ,
									MyGUI::Widget*& ,
									MyGUI::types::TCoord< int > & >(mDelegateDragWidgetInfo) );
			}
		}
	private:
		HandleDragWidgetInfo^ mDelegateDragWidgetInfo;











   	public:
		property Convert<bool>::Type NeedDragDrop
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedDragDrop() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedDragDrop( Convert<bool>::From(_value) );
			}
		}
	


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
