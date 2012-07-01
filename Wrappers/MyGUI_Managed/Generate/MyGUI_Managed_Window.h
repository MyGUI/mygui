/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_TextBox.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class Window : public TextBox
		{

		private:
			typedef MyGUI::Window ThisType;

		public:
			Window() : TextBox() { }

		internal:
			Window( MyGUI::Window* _native ) : TextBox(_native) { }
			Window( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew Window(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

	public:
		delegate void HandleWindowChangeCoord(
			Convert<MyGUI::Window *>::Type _sender );
		event HandleWindowChangeCoord^ EventWindowChangeCoord
		{
			void add(HandleWindowChangeCoord^ _value)
			{
				bool empty = mDelegateWindowChangeCoord == nullptr;

				mDelegateWindowChangeCoord += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventWindowChangeCoord +=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Window * > *>(
								new Delegate1< HandleWindowChangeCoord^ ,
								MyGUI::Window * >(mDelegateWindowChangeCoord) );
			}
			void remove(HandleWindowChangeCoord^ _value)
			{
				mDelegateWindowChangeCoord -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateWindowChangeCoord == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventWindowChangeCoord -=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Window * > *>(
								new Delegate1< HandleWindowChangeCoord^ ,
									MyGUI::Window * >(mDelegateWindowChangeCoord) );
			}
		}
	private:
		HandleWindowChangeCoord^ mDelegateWindowChangeCoord;
	public:
		delegate void HandleWindowButtonPressed(
			Convert<MyGUI::Window *>::Type _sender ,
			Convert<const std::string &>::Type _name );
		event HandleWindowButtonPressed^ EventWindowButtonPressed
		{
			void add(HandleWindowButtonPressed^ _value)
			{
				bool empty = mDelegateWindowButtonPressed == nullptr;

				mDelegateWindowButtonPressed += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventWindowButtonPressed +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Window * ,
							const std::string & > *>(
								new Delegate2< HandleWindowButtonPressed^ ,
								MyGUI::Window * ,
								const std::string & >(mDelegateWindowButtonPressed) );
			}
			void remove(HandleWindowButtonPressed^ _value)
			{
				mDelegateWindowButtonPressed -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateWindowButtonPressed == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventWindowButtonPressed -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Window * ,
							const std::string & > *>(
								new Delegate2< HandleWindowButtonPressed^ ,
									MyGUI::Window * ,
									const std::string & >(mDelegateWindowButtonPressed) );
			}
		}
	private:
		HandleWindowButtonPressed^ mDelegateWindowButtonPressed;
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
		void SetMaxSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaxSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void SetMinSize(
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMinSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	public:
		void DestroySmooth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->destroySmooth( );
		}
	public:
		void SetVisibleSmooth(
			Convert<bool>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setVisibleSmooth(
				Convert<bool>::From(_value) );
		}
	public:
		property Convert<bool>::Type Movable
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getMovable() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMovable( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<const MyGUI::types::TCoord < int > &>::Type ActionScale
		{
			Convert<const MyGUI::types::TCoord < int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TCoord < int > &>::To( static_cast<ThisType*>(mNative)->getActionScale() );
			}
		}
	
	public:
		property Convert<bool>::Type Snap
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getSnap() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSnap( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::types::TSize < int >>::Type MaxSize
		{
			Convert<MyGUI::types::TSize < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize < int >>::To( static_cast<ThisType*>(mNative)->getMaxSize() );
			}
			void set(Convert<const MyGUI::types::TSize < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMaxSize( Convert<const MyGUI::types::TSize < int > &>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::types::TSize < int >>::Type MinSize
		{
			Convert<MyGUI::types::TSize < int >>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::types::TSize < int >>::To( static_cast<ThisType*>(mNative)->getMinSize() );
			}
			void set(Convert<const MyGUI::types::TSize < int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMinSize( Convert<const MyGUI::types::TSize < int > &>::From(_value) );
			}
		}
	
	public:
		property Convert<MyGUI::TextBox *>::Type CaptionWidget
		{
			Convert<MyGUI::TextBox *>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<MyGUI::TextBox *>::To( static_cast<ThisType*>(mNative)->getCaptionWidget() );
			}
		}
	
	public:
		property Convert<bool>::Type AutoAlpha
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getAutoAlpha() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoAlpha( Convert<bool>::From(_value) );
			}
		}
	
		};

	} // namespace Managed
} // namespace MyGUI
