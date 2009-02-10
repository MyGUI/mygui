/*!
	@file
	@author		Generate utility by Albert Semenov
	@date
	@module
*/
#pragma once

#include "../MMyGUI_Core.h"

MMYGUI_BEGIN_NAMESPACE

public ref class IWidget
{

	//--------------------------------------------------------------------
	// объявление типов и конструкторов
	MMYGUI_DECLARE_BASE( IWidget, Widget );

	#include "../MMyGUI_WidgetManual.h"

	//InsertPoint



   	public:
		delegate void HandleActionInfo( Convert<MyGUI::Widget *>::Type _sender, Convert<const std::string &>::Type _key, Convert<const std::string &>::Type _value );
		event HandleActionInfo^ EventActionInfo
		{
			void add(HandleActionInfo^ _value)
			{
				mDelegateActionInfo += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventActionInfo =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, const std::string &, const std::string & > *>(
						new Delegate3< HandleActionInfo^, MyGUI::Widget *, const std::string &, const std::string & >(mDelegateActionInfo) );
			}
			void remove(HandleActionInfo^ _value)
			{
				mDelegateActionInfo -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateActionInfo == nullptr)
					static_cast<ThisType*>(mNative)->eventActionInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventActionInfo =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, const std::string &, const std::string & > *>(
							new Delegate3< HandleActionInfo^, MyGUI::Widget *, const std::string &, const std::string & >(mDelegateActionInfo) );
			}
		}
	private:
		HandleActionInfo^ mDelegateActionInfo;




   	public:
		delegate void HandleToolTip( Convert<MyGUI::Widget *>::Type _sender, Convert<const MyGUI::ToolTipInfo &>::Type _info );
		event HandleToolTip^ EventToolTip
		{
			void add(HandleToolTip^ _value)
			{
				mDelegateToolTip += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventToolTip =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, const MyGUI::ToolTipInfo & > *>(
						new Delegate2< HandleToolTip^, MyGUI::Widget *, const MyGUI::ToolTipInfo & >(mDelegateToolTip) );
			}
			void remove(HandleToolTip^ _value)
			{
				mDelegateToolTip -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateToolTip == nullptr)
					static_cast<ThisType*>(mNative)->eventToolTip = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventToolTip =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, const MyGUI::ToolTipInfo & > *>(
							new Delegate2< HandleToolTip^, MyGUI::Widget *, const MyGUI::ToolTipInfo & >(mDelegateToolTip) );
			}
		}
	private:
		HandleToolTip^ mDelegateToolTip;




   	public:
		delegate void HandleRootKeyChangeFocus( Convert<MyGUI::Widget *>::Type _sender, Convert<bool>::Type _focus );
		event HandleRootKeyChangeFocus^ EventRootKeyChangeFocus
		{
			void add(HandleRootKeyChangeFocus^ _value)
			{
				mDelegateRootKeyChangeFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, bool > *>(
						new Delegate2< HandleRootKeyChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootKeyChangeFocus) );
			}
			void remove(HandleRootKeyChangeFocus^ _value)
			{
				mDelegateRootKeyChangeFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateRootKeyChangeFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, bool > *>(
							new Delegate2< HandleRootKeyChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootKeyChangeFocus) );
			}
		}
	private:
		HandleRootKeyChangeFocus^ mDelegateRootKeyChangeFocus;




   	public:
		delegate void HandleRootMouseChangeFocus( Convert<MyGUI::Widget *>::Type _sender, Convert<bool>::Type _focus );
		event HandleRootMouseChangeFocus^ EventRootMouseChangeFocus
		{
			void add(HandleRootMouseChangeFocus^ _value)
			{
				mDelegateRootMouseChangeFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, bool > *>(
						new Delegate2< HandleRootMouseChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootMouseChangeFocus) );
			}
			void remove(HandleRootMouseChangeFocus^ _value)
			{
				mDelegateRootMouseChangeFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateRootMouseChangeFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, bool > *>(
							new Delegate2< HandleRootMouseChangeFocus^, MyGUI::Widget *, bool >(mDelegateRootMouseChangeFocus) );
			}
		}
	private:
		HandleRootMouseChangeFocus^ mDelegateRootMouseChangeFocus;




   	public:
		delegate void HandleKeyButtonReleased( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::KeyCode>::Type _key );
		event HandleKeyButtonReleased^ EventKeyButtonReleased
		{
			void add(HandleKeyButtonReleased^ _value)
			{
				mDelegateKeyButtonReleased += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventKeyButtonReleased =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::KeyCode > *>(
						new Delegate2< HandleKeyButtonReleased^, MyGUI::Widget *, MyGUI::KeyCode >(mDelegateKeyButtonReleased) );
			}
			void remove(HandleKeyButtonReleased^ _value)
			{
				mDelegateKeyButtonReleased -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateKeyButtonReleased == nullptr)
					static_cast<ThisType*>(mNative)->eventKeyButtonReleased = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeyButtonReleased =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::KeyCode > *>(
							new Delegate2< HandleKeyButtonReleased^, MyGUI::Widget *, MyGUI::KeyCode >(mDelegateKeyButtonReleased) );
			}
		}
	private:
		HandleKeyButtonReleased^ mDelegateKeyButtonReleased;




   	public:
		delegate void HandleKeyButtonPressed( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::KeyCode>::Type _key, Convert<unsigned int>::Type _char );
		event HandleKeyButtonPressed^ EventKeyButtonPressed
		{
			void add(HandleKeyButtonPressed^ _value)
			{
				mDelegateKeyButtonPressed += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventKeyButtonPressed =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, MyGUI::KeyCode, unsigned int > *>(
						new Delegate3< HandleKeyButtonPressed^, MyGUI::Widget *, MyGUI::KeyCode, unsigned int >(mDelegateKeyButtonPressed) );
			}
			void remove(HandleKeyButtonPressed^ _value)
			{
				mDelegateKeyButtonPressed -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateKeyButtonPressed == nullptr)
					static_cast<ThisType*>(mNative)->eventKeyButtonPressed = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeyButtonPressed =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, MyGUI::KeyCode, unsigned int > *>(
							new Delegate3< HandleKeyButtonPressed^, MyGUI::Widget *, MyGUI::KeyCode, unsigned int >(mDelegateKeyButtonPressed) );
			}
		}
	private:
		HandleKeyButtonPressed^ mDelegateKeyButtonPressed;




   	public:
		delegate void HandleKeySetFocus( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::Widget *>::Type _old );
		event HandleKeySetFocus^ EventKeySetFocus
		{
			void add(HandleKeySetFocus^ _value)
			{
				mDelegateKeySetFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventKeySetFocus =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
						new Delegate2< HandleKeySetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeySetFocus) );
			}
			void remove(HandleKeySetFocus^ _value)
			{
				mDelegateKeySetFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateKeySetFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventKeySetFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeySetFocus =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
							new Delegate2< HandleKeySetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeySetFocus) );
			}
		}
	private:
		HandleKeySetFocus^ mDelegateKeySetFocus;




   	public:
		delegate void HandleKeyLostFocus( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::Widget *>::Type _new );
		event HandleKeyLostFocus^ EventKeyLostFocus
		{
			void add(HandleKeyLostFocus^ _value)
			{
				mDelegateKeyLostFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventKeyLostFocus =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
						new Delegate2< HandleKeyLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeyLostFocus) );
			}
			void remove(HandleKeyLostFocus^ _value)
			{
				mDelegateKeyLostFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateKeyLostFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventKeyLostFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventKeyLostFocus =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
							new Delegate2< HandleKeyLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateKeyLostFocus) );
			}
		}
	private:
		HandleKeyLostFocus^ mDelegateKeyLostFocus;




   	public:
		delegate void HandleMouseButtonDoubleClick( Convert<MyGUI::Widget *>::Type _sender );
		event HandleMouseButtonDoubleClick^ EventMouseButtonDoubleClick
		{
			void add(HandleMouseButtonDoubleClick^ _value)
			{
				mDelegateMouseButtonDoubleClick += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick =
					static_cast< MyGUI::delegates::IDelegate1< MyGUI::Widget * > *>(
						new Delegate1< HandleMouseButtonDoubleClick^, MyGUI::Widget * >(mDelegateMouseButtonDoubleClick) );
			}
			void remove(HandleMouseButtonDoubleClick^ _value)
			{
				mDelegateMouseButtonDoubleClick -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseButtonDoubleClick == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick =
						static_cast< MyGUI::delegates::IDelegate1< MyGUI::Widget * > *>(
							new Delegate1< HandleMouseButtonDoubleClick^, MyGUI::Widget * >(mDelegateMouseButtonDoubleClick) );
			}
		}
	private:
		HandleMouseButtonDoubleClick^ mDelegateMouseButtonDoubleClick;




   	public:
		delegate void HandleMouseButtonClick( Convert<MyGUI::Widget *>::Type _sender );
		event HandleMouseButtonClick^ EventMouseButtonClick
		{
			void add(HandleMouseButtonClick^ _value)
			{
				mDelegateMouseButtonClick += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseButtonClick =
					static_cast< MyGUI::delegates::IDelegate1< MyGUI::Widget * > *>(
						new Delegate1< HandleMouseButtonClick^, MyGUI::Widget * >(mDelegateMouseButtonClick) );
			}
			void remove(HandleMouseButtonClick^ _value)
			{
				mDelegateMouseButtonClick -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseButtonClick == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonClick = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonClick =
						static_cast< MyGUI::delegates::IDelegate1< MyGUI::Widget * > *>(
							new Delegate1< HandleMouseButtonClick^, MyGUI::Widget * >(mDelegateMouseButtonClick) );
			}
		}
	private:
		HandleMouseButtonClick^ mDelegateMouseButtonClick;




   	public:
		delegate void HandleMouseButtonReleased( Convert<MyGUI::Widget *>::Type _sender, Convert<int>::Type _left, Convert<int>::Type _top, Convert<MyGUI::MouseButton>::Type _id );
		event HandleMouseButtonReleased^ EventMouseButtonReleased
		{
			void add(HandleMouseButtonReleased^ _value)
			{
				mDelegateMouseButtonReleased += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseButtonReleased =
					static_cast< MyGUI::delegates::IDelegate4< MyGUI::Widget *, int, int, MyGUI::MouseButton > *>(
						new Delegate4< HandleMouseButtonReleased^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonReleased) );
			}
			void remove(HandleMouseButtonReleased^ _value)
			{
				mDelegateMouseButtonReleased -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseButtonReleased == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonReleased = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonReleased =
						static_cast< MyGUI::delegates::IDelegate4< MyGUI::Widget *, int, int, MyGUI::MouseButton > *>(
							new Delegate4< HandleMouseButtonReleased^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonReleased) );
			}
		}
	private:
		HandleMouseButtonReleased^ mDelegateMouseButtonReleased;




   	public:
		delegate void HandleMouseButtonPressed( Convert<MyGUI::Widget *>::Type _sender, Convert<int>::Type _left, Convert<int>::Type _top, Convert<MyGUI::MouseButton>::Type _id );
		event HandleMouseButtonPressed^ EventMouseButtonPressed
		{
			void add(HandleMouseButtonPressed^ _value)
			{
				mDelegateMouseButtonPressed += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseButtonPressed =
					static_cast< MyGUI::delegates::IDelegate4< MyGUI::Widget *, int, int, MyGUI::MouseButton > *>(
						new Delegate4< HandleMouseButtonPressed^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonPressed) );
			}
			void remove(HandleMouseButtonPressed^ _value)
			{
				mDelegateMouseButtonPressed -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseButtonPressed == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseButtonPressed = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseButtonPressed =
						static_cast< MyGUI::delegates::IDelegate4< MyGUI::Widget *, int, int, MyGUI::MouseButton > *>(
							new Delegate4< HandleMouseButtonPressed^, MyGUI::Widget *, int, int, MyGUI::MouseButton >(mDelegateMouseButtonPressed) );
			}
		}
	private:
		HandleMouseButtonPressed^ mDelegateMouseButtonPressed;




   	public:
		delegate void HandleMouseWheel( Convert<MyGUI::Widget *>::Type _sender, Convert<int>::Type _rel );
		event HandleMouseWheel^ EventMouseWheel
		{
			void add(HandleMouseWheel^ _value)
			{
				mDelegateMouseWheel += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseWheel =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, int > *>(
						new Delegate2< HandleMouseWheel^, MyGUI::Widget *, int >(mDelegateMouseWheel) );
			}
			void remove(HandleMouseWheel^ _value)
			{
				mDelegateMouseWheel -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseWheel == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseWheel = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseWheel =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, int > *>(
							new Delegate2< HandleMouseWheel^, MyGUI::Widget *, int >(mDelegateMouseWheel) );
			}
		}
	private:
		HandleMouseWheel^ mDelegateMouseWheel;




   	public:
		delegate void HandleMouseMove( Convert<MyGUI::Widget *>::Type _sender, Convert<int>::Type _left, Convert<int>::Type _top );
		event HandleMouseMove^ EventMouseMove
		{
			void add(HandleMouseMove^ _value)
			{
				mDelegateMouseMove += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseMove =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, int, int > *>(
						new Delegate3< HandleMouseMove^, MyGUI::Widget *, int, int >(mDelegateMouseMove) );
			}
			void remove(HandleMouseMove^ _value)
			{
				mDelegateMouseMove -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseMove == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseMove = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseMove =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, int, int > *>(
							new Delegate3< HandleMouseMove^, MyGUI::Widget *, int, int >(mDelegateMouseMove) );
			}
		}
	private:
		HandleMouseMove^ mDelegateMouseMove;




   	public:
		delegate void HandleMouseDrag( Convert<MyGUI::Widget *>::Type _sender, Convert<int>::Type _left, Convert<int>::Type _top );
		event HandleMouseDrag^ EventMouseDrag
		{
			void add(HandleMouseDrag^ _value)
			{
				mDelegateMouseDrag += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseDrag =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, int, int > *>(
						new Delegate3< HandleMouseDrag^, MyGUI::Widget *, int, int >(mDelegateMouseDrag) );
			}
			void remove(HandleMouseDrag^ _value)
			{
				mDelegateMouseDrag -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseDrag == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseDrag = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseDrag =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::Widget *, int, int > *>(
							new Delegate3< HandleMouseDrag^, MyGUI::Widget *, int, int >(mDelegateMouseDrag) );
			}
		}
	private:
		HandleMouseDrag^ mDelegateMouseDrag;




   	public:
		delegate void HandleMouseSetFocus( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::Widget *>::Type _old );
		event HandleMouseSetFocus^ EventMouseSetFocus
		{
			void add(HandleMouseSetFocus^ _value)
			{
				mDelegateMouseSetFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseSetFocus =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
						new Delegate2< HandleMouseSetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseSetFocus) );
			}
			void remove(HandleMouseSetFocus^ _value)
			{
				mDelegateMouseSetFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseSetFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseSetFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseSetFocus =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
							new Delegate2< HandleMouseSetFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseSetFocus) );
			}
		}
	private:
		HandleMouseSetFocus^ mDelegateMouseSetFocus;




   	public:
		delegate void HandleMouseLostFocus( Convert<MyGUI::Widget *>::Type _sender, Convert<MyGUI::Widget *>::Type _new );
		event HandleMouseLostFocus^ EventMouseLostFocus
		{
			void add(HandleMouseLostFocus^ _value)
			{
				mDelegateMouseLostFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMouseLostFocus =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
						new Delegate2< HandleMouseLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseLostFocus) );
			}
			void remove(HandleMouseLostFocus^ _value)
			{
				mDelegateMouseLostFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMouseLostFocus == nullptr)
					static_cast<ThisType*>(mNative)->eventMouseLostFocus = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMouseLostFocus =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Widget *, MyGUI::Widget * > *>(
							new Delegate2< HandleMouseLostFocus^, MyGUI::Widget *, MyGUI::Widget * >(mDelegateMouseLostFocus) );
			}
		}
	private:
		HandleMouseLostFocus^ mDelegateMouseLostFocus;




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
		void AttachToWidget( Convert< MyGUI::Widget * >::Type _parent )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->attachToWidget( Convert< MyGUI::Widget * >::From(_parent) );
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
		void SetMaskPick( Convert<const std::string &>::Type _filename )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMaskPick( Convert<const std::string &>::From(_filename) );
		}



   	public:
		property bool InheritsPick
		{
			bool get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->isInheritsPick( );
			}
			void set(bool _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsPick(_value);
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
		Convert< MyGUI::Widget * >::Type GetChildAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::Widget * >::To( static_cast<ThisType*>(mNative)->getChildAt( Convert<size_t>::From(_index) ) );
		}



   	public:
		Convert<size_t>::Type GetChildCount( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getChildCount( ) );
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

MMYGUI_END_NAMESPACE
