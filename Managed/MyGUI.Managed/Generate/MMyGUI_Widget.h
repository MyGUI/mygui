/*!
	@file
	@author		Generate utility by Albert Semenov
	@date
	@module
*/
#pragma once

#include "../MMyGUI_Core.h"

namespace MMyGUI
{

	public ref class Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_BASE( Widget );

		//InsertPoint



   	public:
		delegate void HandleGetContainer( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::Widget * &>::Type _value2, Convert<size_t &>::Type _value3 );
		event HandleGetContainer^ GetContainer
		{
			void add(HandleGetContainer^ _value)
			{
				mDelegateGetContainer += _value;
				static_cast<ThisType*>(mNative)->requestGetContainer = new Delegate3< HandleGetContainer^, MyGUI::Widget *, MyGUI::Widget * &, size_t & >(mDelegateGetContainer);
			}
			void remove(HandleGetContainer^ _value)
			{
				mDelegateGetContainer -= _value;
				if (mDelegateGetContainer == nullptr)
					static_cast<ThisType*>(mNative)->requestGetContainer = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestGetContainer = new Delegate3< HandleGetContainer^, MyGUI::Widget *, MyGUI::Widget * &, size_t & >(mDelegateGetContainer);
			}
		}
	private:
		HandleGetContainer^ mDelegateGetContainer;




   	public:
		delegate void HandleActionInfo( Convert<MyGUI::Widget *>::Type _value1, Convert<const std::string &>::Type _value2, Convert<const std::string &>::Type _value3 );
		event HandleActionInfo^ ActionInfo
		{
			void add(HandleActionInfo^ _value)
			{
				mDelegateActionInfo += _value;
				static_cast<ThisType*>(mNative)->eventActionInfo = new Delegate3< HandleActionInfo^, MyGUI::Widget *, const std::string &, const std::string & >(mDelegateActionInfo);
			}
			void remove(HandleActionInfo^ _value)
			{
				mDelegateActionInfo -= _value;
				if (mDelegateActionInfo == nullptr)
					static_cast<ThisType*>(mNative)->eventActionInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventActionInfo = new Delegate3< HandleActionInfo^, MyGUI::Widget *, const std::string &, const std::string & >(mDelegateActionInfo);
			}
		}
	private:
		HandleActionInfo^ mDelegateActionInfo;




   	public:
		delegate void HandleRootKeyChangeFocus( Convert<MyGUI::Widget *>::Type _value1, Convert<bool>::Type _value2 );
		event HandleRootKeyChangeFocus^ RootKeyChangeFocus
		{
			void add(HandleRootKeyChangeFocus^ _value)
			{
				mDelegateRootKeyChangeFocus += _value;
				static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus = new Delegate2< HandleRootKeyChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootKeyChangeFocus);
			}
			void remove(HandleRootKeyChangeFocus^ _value)
			{
				mDelegateRootKeyChangeFocus -= _value;
				if (mDelegateRootKeyChangeFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus = new Delegate2< HandleRootKeyChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootKeyChangeFocus);
			}
		}
	private:
		HandleRootKeyChangeFocus^ mDelegateRootKeyChangeFocus;




   	public:
		delegate void HandleRootMouseChangeFocus( Convert<MyGUI::Widget *>::Type _value1, Convert<bool>::Type _value2 );
		event HandleRootMouseChangeFocus^ RootMouseChangeFocus
		{
			void add(HandleRootMouseChangeFocus^ _value)
			{
				mDelegateRootMouseChangeFocus += _value;
				static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus = new Delegate2< HandleRootMouseChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootMouseChangeFocus);
			}
			void remove(HandleRootMouseChangeFocus^ _value)
			{
				mDelegateRootMouseChangeFocus -= _value;
				if (mDelegateRootMouseChangeFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus = new Delegate2< HandleRootMouseChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootMouseChangeFocus);
			}
		}
	private:
		HandleRootMouseChangeFocus^ mDelegateRootMouseChangeFocus;




   	public:
		delegate void HandleKeyButtonReleased( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::KeyCode>::Type _value2 );
		event HandleKeyButtonReleased^ KeyButtonReleased
		{
			void add(HandleKeyButtonReleased^ _value)
			{
				mDelegateKeyButtonReleased += _value;
				static_cast<ThisType*>(mNative)->eventKeyButtonReleased = new Delegate2< HandleKeyButtonReleased^, MyGUI::Widget *, MyGUI::KeyCode >(mDelegateKeyButtonReleased);
			}
			void remove(HandleKeyButtonReleased^ _value)
			{
				mDelegateKeyButtonReleased -= _value;
				if (mDelegateKeyButtonReleased == nullptr)
					static_cast<ThisType*>(mNative)->eventKeyButtonReleased = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeyButtonReleased = new Delegate2< HandleKeyButtonReleased^, MyGUI::Widget *, MyGUI::KeyCode >(mDelegateKeyButtonReleased);
			}
		}
	private:
		HandleKeyButtonReleased^ mDelegateKeyButtonReleased;




   	public:
		delegate void HandleKeyButtonPressed( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::KeyCode>::Type _value2, Convert<unsigned int>::Type _value3 );
		event HandleKeyButtonPressed^ KeyButtonPressed
		{
			void add(HandleKeyButtonPressed^ _value)
			{
				mDelegateKeyButtonPressed += _value;
				static_cast<ThisType*>(mNative)->eventKeyButtonPressed = new Delegate3< HandleKeyButtonPressed^, MyGUI::Widget *, MyGUI::KeyCode, unsigned int >(mDelegateKeyButtonPressed);
			}
			void remove(HandleKeyButtonPressed^ _value)
			{
				mDelegateKeyButtonPressed -= _value;
				if (mDelegateKeyButtonPressed == nullptr)
					static_cast<ThisType*>(mNative)->eventKeyButtonPressed = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeyButtonPressed = new Delegate3< HandleKeyButtonPressed^, MyGUI::Widget *, MyGUI::KeyCode, unsigned int >(mDelegateKeyButtonPressed);
			}
		}
	private:
		HandleKeyButtonPressed^ mDelegateKeyButtonPressed;




   	public:
		delegate void HandleKeySetFocus( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::Widget *>::Type _value2 );
		event HandleKeySetFocus^ KeySetFocus
		{
			void add(HandleKeySetFocus^ _value)
			{
				mDelegateKeySetFocus += _value;
				static_cast<ThisType*>(mNative)->eventKeySetFocus = new Delegate2< HandleKeySetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeySetFocus);
			}
			void remove(HandleKeySetFocus^ _value)
			{
				mDelegateKeySetFocus -= _value;
				if (mDelegateKeySetFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventKeySetFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeySetFocus = new Delegate2< HandleKeySetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeySetFocus);
			}
		}
	private:
		HandleKeySetFocus^ mDelegateKeySetFocus;




   	public:
		delegate void HandleKeyLostFocus( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::Widget *>::Type _value2 );
		event HandleKeyLostFocus^ KeyLostFocus
		{
			void add(HandleKeyLostFocus^ _value)
			{
				mDelegateKeyLostFocus += _value;
				static_cast<ThisType*>(mNative)->eventKeyLostFocus = new Delegate2< HandleKeyLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeyLostFocus);
			}
			void remove(HandleKeyLostFocus^ _value)
			{
				mDelegateKeyLostFocus -= _value;
				if (mDelegateKeyLostFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventKeyLostFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeyLostFocus = new Delegate2< HandleKeyLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeyLostFocus);
			}
		}
	private:
		HandleKeyLostFocus^ mDelegateKeyLostFocus;




   	public:
		delegate void HandleMouseButtonDoubleClick( Convert<MyGUI::Widget *>::Type _value1 );
		event HandleMouseButtonDoubleClick^ MouseButtonDoubleClick
		{
			void add(HandleMouseButtonDoubleClick^ _value)
			{
				mDelegateMouseButtonDoubleClick += _value;
				static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick = new Delegate1< HandleMouseButtonDoubleClick^, MyGUI::Widget * >(mDelegateMouseButtonDoubleClick);
			}
			void remove(HandleMouseButtonDoubleClick^ _value)
			{
				mDelegateMouseButtonDoubleClick -= _value;
				if (mDelegateMouseButtonDoubleClick == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick = new Delegate1< HandleMouseButtonDoubleClick^, MyGUI::Widget * >(mDelegateMouseButtonDoubleClick);
			}
		}
	private:
		HandleMouseButtonDoubleClick^ mDelegateMouseButtonDoubleClick;




   	public:
		delegate void HandleMouseButtonClick( Convert<MyGUI::Widget *>::Type _value1 );
		event HandleMouseButtonClick^ MouseButtonClick
		{
			void add(HandleMouseButtonClick^ _value)
			{
				mDelegateMouseButtonClick += _value;
				static_cast<ThisType*>(mNative)->eventMouseButtonClick = new Delegate1< HandleMouseButtonClick^, MyGUI::Widget * >(mDelegateMouseButtonClick);
			}
			void remove(HandleMouseButtonClick^ _value)
			{
				mDelegateMouseButtonClick -= _value;
				if (mDelegateMouseButtonClick == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonClick = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonClick = new Delegate1< HandleMouseButtonClick^, MyGUI::Widget * >(mDelegateMouseButtonClick);
			}
		}
	private:
		HandleMouseButtonClick^ mDelegateMouseButtonClick;




   	public:
		delegate void HandleMouseButtonReleased( Convert<MyGUI::Widget *>::Type _value1, Convert<int>::Type _value2, Convert<int>::Type _value3, Convert<MyGUI::MouseButton>::Type _value4 );
		event HandleMouseButtonReleased^ MouseButtonReleased
		{
			void add(HandleMouseButtonReleased^ _value)
			{
				mDelegateMouseButtonReleased += _value;
				static_cast<ThisType*>(mNative)->eventMouseButtonReleased = new Delegate4< HandleMouseButtonReleased^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonReleased);
			}
			void remove(HandleMouseButtonReleased^ _value)
			{
				mDelegateMouseButtonReleased -= _value;
				if (mDelegateMouseButtonReleased == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonReleased = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonReleased = new Delegate4< HandleMouseButtonReleased^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonReleased);
			}
		}
	private:
		HandleMouseButtonReleased^ mDelegateMouseButtonReleased;




   	public:
		delegate void HandleMouseButtonPressed( Convert<MyGUI::Widget *>::Type _value1, Convert<int>::Type _value2, Convert<int>::Type _value3, Convert<MyGUI::MouseButton>::Type _value4 );
		event HandleMouseButtonPressed^ MouseButtonPressed
		{
			void add(HandleMouseButtonPressed^ _value)
			{
				mDelegateMouseButtonPressed += _value;
				static_cast<ThisType*>(mNative)->eventMouseButtonPressed = new Delegate4< HandleMouseButtonPressed^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonPressed);
			}
			void remove(HandleMouseButtonPressed^ _value)
			{
				mDelegateMouseButtonPressed -= _value;
				if (mDelegateMouseButtonPressed == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonPressed = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonPressed = new Delegate4< HandleMouseButtonPressed^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonPressed);
			}
		}
	private:
		HandleMouseButtonPressed^ mDelegateMouseButtonPressed;




   	public:
		delegate void HandleMouseWheel( Convert<MyGUI::Widget *>::Type _value1, Convert<int>::Type _value2 );
		event HandleMouseWheel^ MouseWheel
		{
			void add(HandleMouseWheel^ _value)
			{
				mDelegateMouseWheel += _value;
				static_cast<ThisType*>(mNative)->eventMouseWheel = new Delegate2< HandleMouseWheel^, MyGUI::Widget *, int >(mDelegateMouseWheel);
			}
			void remove(HandleMouseWheel^ _value)
			{
				mDelegateMouseWheel -= _value;
				if (mDelegateMouseWheel == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseWheel = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseWheel = new Delegate2< HandleMouseWheel^, MyGUI::Widget *, int >(mDelegateMouseWheel);
			}
		}
	private:
		HandleMouseWheel^ mDelegateMouseWheel;




   	public:
		delegate void HandleMouseMove( Convert<MyGUI::Widget *>::Type _value1, Convert<int>::Type _value2, Convert<int>::Type _value3 );
		event HandleMouseMove^ MouseMove
		{
			void add(HandleMouseMove^ _value)
			{
				mDelegateMouseMove += _value;
				static_cast<ThisType*>(mNative)->eventMouseMove = new Delegate3< HandleMouseMove^, MyGUI::Widget *, int, int >(mDelegateMouseMove);
			}
			void remove(HandleMouseMove^ _value)
			{
				mDelegateMouseMove -= _value;
				if (mDelegateMouseMove == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseMove = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseMove = new Delegate3< HandleMouseMove^, MyGUI::Widget *, int, int >(mDelegateMouseMove);
			}
		}
	private:
		HandleMouseMove^ mDelegateMouseMove;




   	public:
		delegate void HandleMouseDrag( Convert<MyGUI::Widget *>::Type _value1, Convert<int>::Type _value2, Convert<int>::Type _value3 );
		event HandleMouseDrag^ MouseDrag
		{
			void add(HandleMouseDrag^ _value)
			{
				mDelegateMouseDrag += _value;
				static_cast<ThisType*>(mNative)->eventMouseDrag = new Delegate3< HandleMouseDrag^, MyGUI::Widget *, int, int >(mDelegateMouseDrag);
			}
			void remove(HandleMouseDrag^ _value)
			{
				mDelegateMouseDrag -= _value;
				if (mDelegateMouseDrag == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseDrag = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseDrag = new Delegate3< HandleMouseDrag^, MyGUI::Widget *, int, int >(mDelegateMouseDrag);
			}
		}
	private:
		HandleMouseDrag^ mDelegateMouseDrag;




   	public:
		delegate void HandleMouseSetFocus( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::Widget *>::Type _value2 );
		event HandleMouseSetFocus^ MouseSetFocus
		{
			void add(HandleMouseSetFocus^ _value)
			{
				mDelegateMouseSetFocus += _value;
				static_cast<ThisType*>(mNative)->eventMouseSetFocus = new Delegate2< HandleMouseSetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseSetFocus);
			}
			void remove(HandleMouseSetFocus^ _value)
			{
				mDelegateMouseSetFocus -= _value;
				if (mDelegateMouseSetFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseSetFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseSetFocus = new Delegate2< HandleMouseSetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseSetFocus);
			}
		}
	private:
		HandleMouseSetFocus^ mDelegateMouseSetFocus;




   	public:
		delegate void HandleMouseLostFocus( Convert<MyGUI::Widget *>::Type _value1, Convert<MyGUI::Widget *>::Type _value2 );
		event HandleMouseLostFocus^ MouseLostFocus
		{
			void add(HandleMouseLostFocus^ _value)
			{
				mDelegateMouseLostFocus += _value;
				static_cast<ThisType*>(mNative)->eventMouseLostFocus = new Delegate2< HandleMouseLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseLostFocus);
			}
			void remove(HandleMouseLostFocus^ _value)
			{
				mDelegateMouseLostFocus -= _value;
				if (mDelegateMouseLostFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseLostFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseLostFocus = new Delegate2< HandleMouseLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseLostFocus);
			}
		}
	private:
		HandleMouseLostFocus^ mDelegateMouseLostFocus;




   	public:
		void SetUserData( Convert< MyGUI::Any >::Type _data )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setUserData( Convert< MyGUI::Any >::From(_data) );
		}



   	public:
		void ClearUserStrings( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clearUserStrings( );
		}



   	public:
		Convert<bool>::Type IsUserString( Convert<const std::string &>::Type _key )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isUserString( Convert<const std::string &>::From(_key) ) );
		}



   	public:
		Convert<bool>::Type ClearUserString( Convert<const std::string &>::Type _key )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->clearUserString( Convert<const std::string &>::From(_key) ) );
		}



   	public:
		Convert<const std::string &>::Type GetUserString( Convert<const std::string &>::Type _key )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getUserString( Convert<const std::string &>::From(_key) ) );
		}



   	public:
		void SetUserString( Convert<const std::string &>::Type _key, Convert<const std::string &>::Type _value )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setUserString( Convert<const std::string &>::From(_key), Convert<const std::string &>::From(_value) );
		}



   	public:
		Convert<int>::Type GetMarginBottom( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getMarginBottom( ) );
		}



   	public:
		Convert<int>::Type GetMarginTop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getMarginTop( ) );
		}



   	public:
		Convert<int>::Type GetMarginRight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getMarginRight( ) );
		}



   	public:
		Convert<int>::Type GetMarginLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getMarginLeft( ) );
		}



   	public:
		Convert<const MyGUI::types::TRect< int > &>::Type GetMargin( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::types::TRect< int > &>::To( static_cast<ThisType*>(mNative)->getMargin( ) );
		}



   	public:
		Convert<int>::Type GetViewHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getViewHeight( ) );
		}



   	public:
		Convert<int>::Type GetViewWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getViewWidth( ) );
		}



   	public:
		Convert<int>::Type GetViewBottom( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getViewBottom( ) );
		}



   	public:
		Convert<int>::Type GetViewTop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getViewTop( ) );
		}



   	public:
		Convert<int>::Type GetViewRight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getViewRight( ) );
		}



   	public:
		Convert<int>::Type GetViewLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getViewLeft( ) );
		}



   	public:
		Convert<int>::Type GetHeight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getHeight( ) );
		}



   	public:
		Convert<int>::Type GetWidth( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getWidth( ) );
		}



   	public:
		Convert<int>::Type GetBottom( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getBottom( ) );
		}



   	public:
		Convert<int>::Type GetTop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getTop( ) );
		}



   	public:
		Convert<int>::Type GetRight( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getRight( ) );
		}



   	public:
		Convert<int>::Type GetLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getLeft( ) );
		}



   	public:
		Convert<bool>::Type IsMargin( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isMargin( ) );
		}



   	public:
		property Convert< MyGUI::Align >::Type Align
		{
			Convert< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::Align >::To( static_cast<ThisType*>(mNative)->getAlign() );
			}
			void set(Convert< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAlign( Convert< MyGUI::Align >::From(_value) );
			}
		}



   	public:
		Convert<int>::Type GetAbsoluteTop( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getAbsoluteTop( ) );
		}



   	public:
		Convert<int>::Type GetAbsoluteLeft( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<int>::To( static_cast<ThisType*>(mNative)->getAbsoluteLeft( ) );
		}



   	public:
		Convert< MyGUI::types::TCoord< int > >::Type GetAbsoluteCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast<ThisType*>(mNative)->getAbsoluteCoord( ) );
		}



   	public:
		Convert< MyGUI::types::TRect< int > >::Type GetAbsoluteRect( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TRect< int > >::To( static_cast<ThisType*>(mNative)->getAbsoluteRect( ) );
		}



   	public:
		Convert<const MyGUI::types::TPoint< int > &>::Type GetAbsolutePosition( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const MyGUI::types::TPoint< int > &>::To( static_cast<ThisType*>(mNative)->getAbsolutePosition( ) );
		}



   	public:
		property Convert< MyGUI::WidgetStyle >::Type WidgetStyle
		{
			Convert< MyGUI::WidgetStyle >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::WidgetStyle >::To( static_cast<ThisType*>(mNative)->getWidgetStyle() );
			}
			void set(Convert< MyGUI::WidgetStyle >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setWidgetStyle( Convert< MyGUI::WidgetStyle >::From(_value) );
			}
		}



   	public:
		void ChangeWidgetSkin( Convert<const std::string &>::Type _skinname )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->changeWidgetSkin( Convert<const std::string &>::From(_skinname) );
		}



   	public:
		void AttachToWidget( Convert< MyGUI::Widget * >::Type _widget )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->attachToWidget( Convert< MyGUI::Widget * >::From(_widget) );
		}



   	public:
		void DetachFromWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->detachFromWidget( );
		}



   	public:
		property Convert<bool>::Type EnableToolTip
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getEnableToolTip() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEnableToolTip( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type NeedToolTip
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedToolTip() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedToolTip( Convert<bool>::From(_value) );
			}
		}



   	public:
		Convert< MyGUI::ISubWidgetRect *>::Type GetSubWidgetMain( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::ISubWidgetRect *>::To( static_cast<ThisType*>(mNative)->getSubWidgetMain( ) );
		}



   	public:
		Convert< MyGUI::ISubWidgetText *>::Type GetSubWidgetText( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::ISubWidgetText *>::To( static_cast<ThisType*>(mNative)->getSubWidgetText( ) );
		}



   	public:
		Convert< MyGUI::Widget * >::Type GetClientWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getClientWidget( ) );
		}



   	public:
		Convert< MyGUI::types::TCoord< int > >::Type GetClientCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast<ThisType*>(mNative)->getClientCoord( ) );
		}



   	public:
		Convert<const std::string &>::Type GetLayerName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getLayerName( ) );
		}



   	public:
		property Convert<const std::string &>::Type Pointer
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getPointer() );
			}
			void set(Convert<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPointer( Convert<const std::string &>::From(_value) );
			}
		}



   	public:
		void SetEnabledSilent( Convert<bool>::Type _enabled )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setEnabledSilent( Convert<bool>::From(_enabled) );
		}



   	public:
		property bool Enabled
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isEnabled( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setEnabled(_value);
			}
		}



   	public:
		void SetMaskPeek( Convert<const std::string &>::Type _filename )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaskPeek( Convert<const std::string &>::From(_filename) );
		}



   	public:
		property bool InheritsPeek
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isInheritsPeek( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsPeek(_value);
			}
		}



   	public:
		property bool NeedMouseFocus
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isNeedMouseFocus( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedMouseFocus(_value);
			}
		}



   	public:
		property bool NeedKeyFocus
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isNeedKeyFocus( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedKeyFocus(_value);
			}
		}



   	public:
		Convert< MyGUI::Widget * >::Type FindWidget( Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->findWidget( Convert<const std::string &>::From(_name) ) );
		}



   	public:
		Convert< MyGUI::Enumerator<  std::vector<  MyGUI::Widget *  >  > >::Type GetEnumerator( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Enumerator<  std::vector<  MyGUI::Widget *  >  > >::To( static_cast<ThisType*>(mNative)->getEnumerator( ) );
		}



   	public:
		Convert< MyGUI::Widget * >::Type GetParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getParent( ) );
		}



   	public:
		Convert<bool>::Type IsRootWidget( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->isRootWidget( ) );
		}



   	public:
		Convert<bool>::Type SetState( Convert<const std::string &>::Type _state )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->setState( Convert<const std::string &>::From(_state) ) );
		}



   	public:
		property bool InheritsAlpha
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isInheritsAlpha( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsAlpha(_value);
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
		Convert< MyGUI::types::TSize< int > >::Type GetTextSize( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TSize< int > >::To( static_cast<ThisType*>(mNative)->getTextSize( ) );
		}



   	public:
		Convert< MyGUI::types::TCoord< int > >::Type GetTextCoord( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast<ThisType*>(mNative)->getTextCoord( ) );
		}



   	public:
		property Convert< MyGUI::Align >::Type TextAlign
		{
			Convert< MyGUI::Align >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::Align >::To( static_cast<ThisType*>(mNative)->getTextAlign() );
			}
			void set(Convert< MyGUI::Align >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setTextAlign( Convert< MyGUI::Align >::From(_value) );
			}
		}



   	public:
		property Convert< unsigned int >::Type FontHeight
		{
			Convert< unsigned int >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< unsigned int >::To( static_cast<ThisType*>(mNative)->getFontHeight() );
			}
			void set(Convert< unsigned int >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontHeight( Convert< unsigned int >::From(_value) );
			}
		}



   	public:
		property Convert<const std::string &>::Type FontName
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getFontName() );
			}
			void set(Convert<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setFontName( Convert<const std::string &>::From(_value) );
			}
		}



   	public:
		property Convert<const MyGUI::Colour &>::Type Colour
		{
			Convert<const MyGUI::Colour &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::Colour &>::To( static_cast<ThisType*>(mNative)->getColour() );
			}
			void set(Convert<const MyGUI::Colour &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setColour( Convert<const MyGUI::Colour &>::From(_value) );
			}
		}



   	public:
		property Convert<const Ogre::UTFString &>::Type Caption
		{
			Convert<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getCaption() );
			}
			void set(Convert<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCaption( Convert<const Ogre::UTFString &>::From(_value) );
			}
		}



   	public:
		property bool Visible
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isVisible( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setVisible(_value);
			}
		}



   	public:
		void SetRealCoord( Convert<float>::Type _left, Convert<float>::Type _top, Convert<float>::Type _width, Convert<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord( Convert<float>::From(_left), Convert<float>::From(_top), Convert<float>::From(_width), Convert<float>::From(_height) );
		}



   	public:
		void SetRealSize( Convert<float>::Type _width, Convert<float>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize( Convert<float>::From(_width), Convert<float>::From(_height) );
		}



   	public:
		void SetRealPosition( Convert<float>::Type _left, Convert<float>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition( Convert<float>::From(_left), Convert<float>::From(_top) );
		}



   	public:
		void SetRealCoord( Convert<const MyGUI::types::TCoord< float > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealCoord( Convert<const MyGUI::types::TCoord< float > &>::From(_coord) );
		}



   	public:
		void SetRealSize( Convert<const MyGUI::types::TSize< float > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealSize( Convert<const MyGUI::types::TSize< float > &>::From(_size) );
		}



   	public:
		void SetRealPosition( Convert<const MyGUI::types::TPoint< float > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRealPosition( Convert<const MyGUI::types::TPoint< float > &>::From(_point) );
		}



   	public:
		void SetCoord( Convert<int>::Type _left, Convert<int>::Type _top, Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( Convert<int>::From(_left), Convert<int>::From(_top), Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetSize( Convert<int>::Type _width, Convert<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( Convert<int>::From(_width), Convert<int>::From(_height) );
		}



   	public:
		void SetPosition( Convert<int>::Type _left, Convert<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( Convert<int>::From(_left), Convert<int>::From(_top) );
		}



   	public:
		property Convert<const MyGUI::types::TCoord< int > &>::Type Coord
		{
			Convert<const MyGUI::types::TCoord< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TCoord< int > &>::To( static_cast<ThisType*>(mNative)->getCoord() );
			}
			void set(Convert<const MyGUI::types::TCoord< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCoord( Convert<const MyGUI::types::TCoord< int > &>::From(_value) );
			}
		}



   	public:
		property Convert<const MyGUI::types::TSize< int > &>::Type Size
		{
			Convert<const MyGUI::types::TSize< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TSize< int > &>::To( static_cast<ThisType*>(mNative)->getSize() );
			}
			void set(Convert<const MyGUI::types::TSize< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setSize( Convert<const MyGUI::types::TSize< int > &>::From(_value) );
			}
		}



   	public:
		property Convert<const MyGUI::types::TPoint< int > &>::Type Position
		{
			Convert<const MyGUI::types::TPoint< int > &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const MyGUI::types::TPoint< int > &>::To( static_cast<ThisType*>(mNative)->getPosition() );
			}
			void set(Convert<const MyGUI::types::TPoint< int > &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setPosition( Convert<const MyGUI::types::TPoint< int > &>::From(_value) );
			}
		}



   	public:
		Convert<const std::string &>::Type GetName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getName( ) );
		}






   	public:
		Convert< MyGUI::Widget * >::Type CreateWidgetRealT( Convert<const std::string &>::Type _type, Convert<const std::string &>::Type _skin, Convert<const MyGUI::types::TCoord< float > &>::Type _coord, Convert< MyGUI::Align >::Type _align, Convert<const std::string &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->createWidgetRealT( Convert<const std::string &>::From(_type), Convert<const std::string &>::From(_skin), Convert<const MyGUI::types::TCoord< float > &>::From(_coord), Convert< MyGUI::Align >::From(_align), Convert<const std::string &>::From(_name) ) );
		}


   	public:
		Convert< MyGUI::Widget * >::Type CreateWidgetRealT( Convert<const std::string &>::Type _type, Convert<const std::string &>::Type _skin, Convert<const MyGUI::types::TCoord< float > &>::Type _coord, Convert< MyGUI::Align >::Type _align )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->createWidgetRealT( Convert<const std::string &>::From(_type), Convert<const std::string &>::From(_skin), Convert<const MyGUI::types::TCoord< float > &>::From(_coord), Convert< MyGUI::Align >::From(_align) ) );
		}



	};

} // namespace MMyGUI
