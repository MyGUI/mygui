/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_WidgetInput.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class Widget : public WidgetInput
		{

		private:
			typedef MyGUI::Widget ThisType;

		public:
			Widget() : WidgetInput() { }

		internal:
			Widget( MyGUI::Widget* _native ) : WidgetInput(_native) { }
			Widget( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew Widget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleChangeProperty(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value );
		event HandleChangeProperty^ EventChangeProperty
		{
			void add(HandleChangeProperty^ _value)
			{
				bool empty = mDelegateChangeProperty == nullptr;

				mDelegateChangeProperty += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventChangeProperty +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::Widget * ,
							const std::string & ,
							const std::string & > *>(
								new Delegate3< HandleChangeProperty^ ,
								MyGUI::Widget * ,
								const std::string & ,
								const std::string & >(mDelegateChangeProperty) );
			}
			void remove(HandleChangeProperty^ _value)
			{
				mDelegateChangeProperty -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateChangeProperty == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventChangeProperty -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::Widget * ,
							const std::string & ,
							const std::string & > *>(
								new Delegate3< HandleChangeProperty^ ,
									MyGUI::Widget * ,
									const std::string & ,
									const std::string & >(mDelegateChangeProperty) );
			}
		}
	private:
		HandleChangeProperty^ mDelegateChangeProperty;
	public:
		void SetProperty(
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setProperty(
				Convert<const std::string &>::From(_key) ,
				Convert<const std::string &>::From(_value) );
		}
	public:
		void SetWidgetStyle(
			Convert<MyGUI::WidgetStyle>::Type _style ,
			Convert<const std::string &>::Type _layer )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setWidgetStyle(
				Convert<MyGUI::WidgetStyle>::From(_style) ,
				Convert<const std::string &>::From(_layer) );
		}
	public:
		void ChangeWidgetSkin(
			Convert<const std::string &>::Type _skinName )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->changeWidgetSkin(
				Convert<const std::string &>::From(_skinName) );
		}
	public:
		void AttachToWidget(
			Convert<MyGUI::Widget *>::Type _parent ,
			Convert<MyGUI::WidgetStyle>::Type _style ,
			Convert<const std::string &>::Type _layer )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->attachToWidget(
				Convert<MyGUI::Widget *>::From(_parent) ,
				Convert<MyGUI::WidgetStyle>::From(_style) ,
				Convert<const std::string &>::From(_layer) );
		}
	public:
		void DetachFromWidget(
			Convert<const std::string &>::Type _layer )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->detachFromWidget(
				Convert<const std::string &>::From(_layer) );
		}
	public:
		void SetEnabledSilent(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setEnabledSilent(
				Convert<bool>::From(_value) );
		}
	public:
		Convert<MyGUI::Widget *>::Type FindWidget(
			Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->findWidget(
					Convert<const std::string &>::From(_name) ) );
		}
	public:
		Convert<MyGUI::Widget *>::Type GetChildAt(
			Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Widget *>::To(
				static_cast<ThisType*>(mNative)->getChildAt(
					Convert<size_t>::From(_index) ) );
		}
	public:
		void SetColour(
			Convert<const MyGUI::Colour &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setColour(
				Convert<const MyGUI::Colour &>::From(_value) );
		}
	public:
		void SetRealCoord(
			Convert<float>::Type _left ,
			Convert<float>::Type _top ,
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord(
				Convert<float>::From(_left) ,
				Convert<float>::From(_top) ,
				Convert<float>::From(_width) ,
				Convert<float>::From(_height) );
		}
	public:
		void SetRealSize(
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize(
				Convert<float>::From(_width) ,
				Convert<float>::From(_height) );
		}
	public:
		void SetRealPosition(
			Convert<float>::Type _left ,
			Convert<float>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition(
				Convert<float>::From(_left) ,
				Convert<float>::From(_top) );
		}
	public:
		void SetRealCoord(
			Convert<const MyGUI::types::TCoord < float > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord(
				Convert<const MyGUI::types::TCoord < float > &>::From(_value) );
		}
	public:
		void SetRealSize(
			Convert<const MyGUI::types::TSize < float > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize(
				Convert<const MyGUI::types::TSize < float > &>::From(_value) );
		}
	public:
		void SetRealPosition(
			Convert<const MyGUI::types::TPoint < float > &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition(
				Convert<const MyGUI::types::TPoint < float > &>::From(_value) );
		}
	public:
		void SetCoord(
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) ,
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetPosition(
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) );
		}
	public:
		property Convert<MyGUI::WidgetStyle>::Type WidgetStyle
		{
			Convert<MyGUI::WidgetStyle>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::WidgetStyle>::To( static_cast<ThisType*>(mNative)->getWidgetStyle() );
			}
		}
	
	public:
		property Convert<MyGUI::Widget *>::Type ClientWidget
		{
			Convert<MyGUI::Widget *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Widget *>::To( static_cast<ThisType*>(mNative)->getClientWidget() );
			}
		}
	
	public:
		property Convert<MyGUI::types::TCoord < int >>::Type ClientCoord
		{
			Convert<MyGUI::types::TCoord < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TCoord < int >>::To( static_cast<ThisType*>(mNative)->getClientCoord() );
			}
		}
	
	public:
		property Convert<bool>::Type InheritedEnabled
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getInheritedEnabled() );
			}
		}
	
	public:
		property Convert<bool>::Type Enabled
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEnabled() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEnabled( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<size_t>::Type ChildCount
		{
			Convert<size_t>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getChildCount() );
			}
		}
	
	public:
		property Convert<MyGUI::types::TSize < int >>::Type ParentSize
		{
			Convert<MyGUI::types::TSize < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize < int >>::To( static_cast<ThisType*>(mNative)->getParentSize() );
			}
		}
	
	public:
		property Convert<MyGUI::Widget *>::Type Parent
		{
			Convert<MyGUI::Widget *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Widget *>::To( static_cast<ThisType*>(mNative)->getParent() );
			}
		}
	
	public:
		property Convert<bool>::Type IsRootWidget
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->isRootWidget() );
			}
		}
	
	public:
		property Convert<bool>::Type InheritsAlpha
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getInheritsAlpha() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsAlpha( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<float>::Type Alpha
		{
			Convert<float>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<float>::To( static_cast<ThisType*>(mNative)->getAlpha() );
			}
			void set(Convert<float>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAlpha( Convert<float>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::Align>::Type Align
		{
			Convert<MyGUI::Align>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::Align>::To( static_cast<ThisType*>(mNative)->getAlign() );
			}
			void set(Convert<MyGUI::Align>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAlign( Convert<MyGUI::Align>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type InheritedVisible
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getInheritedVisible() );
			}
		}
	
	public:
		property Convert<bool>::Type Visible
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getVisible() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisible( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<const std::string &>::Type Name
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getName() );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
