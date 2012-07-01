/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI_Managed_WidgetUserData.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class WidgetInput abstract : public WidgetUserData
		{

		private:
			typedef MyGUI::Widget ThisType;

		public:
			WidgetInput() : WidgetUserData() { }

		internal:
			WidgetInput( MyGUI::Widget* _native ) : WidgetUserData(_native) { }

			//InsertPoint

	public:
		delegate void HandleToolTip(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<const MyGUI::ToolTipInfo &>::Type _info );
		event HandleToolTip^ EventToolTip
		{
			void add(HandleToolTip^ _value)
			{
				bool empty = mDelegateToolTip == nullptr;

				mDelegateToolTip += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventToolTip +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							const MyGUI::ToolTipInfo & > *>(
								new Delegate2< HandleToolTip^ ,
								MyGUI::Widget * ,
								const MyGUI::ToolTipInfo & >(mDelegateToolTip) );
			}
			void remove(HandleToolTip^ _value)
			{
				mDelegateToolTip -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateToolTip == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventToolTip -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							const MyGUI::ToolTipInfo & > *>(
								new Delegate2< HandleToolTip^ ,
									MyGUI::Widget * ,
									const MyGUI::ToolTipInfo & >(mDelegateToolTip) );
			}
		}
	private:
		HandleToolTip^ mDelegateToolTip;
	public:
		delegate void HandleRootKeyChangeFocus(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<bool>::Type _focus );
		event HandleRootKeyChangeFocus^ EventRootKeyChangeFocus
		{
			void add(HandleRootKeyChangeFocus^ _value)
			{
				bool empty = mDelegateRootKeyChangeFocus == nullptr;

				mDelegateRootKeyChangeFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							bool > *>(
								new Delegate2< HandleRootKeyChangeFocus^ ,
								MyGUI::Widget * ,
								bool >(mDelegateRootKeyChangeFocus) );
			}
			void remove(HandleRootKeyChangeFocus^ _value)
			{
				mDelegateRootKeyChangeFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateRootKeyChangeFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventRootKeyChangeFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							bool > *>(
								new Delegate2< HandleRootKeyChangeFocus^ ,
									MyGUI::Widget * ,
									bool >(mDelegateRootKeyChangeFocus) );
			}
		}
	private:
		HandleRootKeyChangeFocus^ mDelegateRootKeyChangeFocus;
	public:
		delegate void HandleRootMouseChangeFocus(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<bool>::Type _focus );
		event HandleRootMouseChangeFocus^ EventRootMouseChangeFocus
		{
			void add(HandleRootMouseChangeFocus^ _value)
			{
				bool empty = mDelegateRootMouseChangeFocus == nullptr;

				mDelegateRootMouseChangeFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							bool > *>(
								new Delegate2< HandleRootMouseChangeFocus^ ,
								MyGUI::Widget * ,
								bool >(mDelegateRootMouseChangeFocus) );
			}
			void remove(HandleRootMouseChangeFocus^ _value)
			{
				mDelegateRootMouseChangeFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateRootMouseChangeFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventRootMouseChangeFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							bool > *>(
								new Delegate2< HandleRootMouseChangeFocus^ ,
									MyGUI::Widget * ,
									bool >(mDelegateRootMouseChangeFocus) );
			}
		}
	private:
		HandleRootMouseChangeFocus^ mDelegateRootMouseChangeFocus;
	public:
		delegate void HandleKeyButtonReleased(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<MyGUI::KeyCode>::Type _key );
		event HandleKeyButtonReleased^ EventKeyButtonReleased
		{
			void add(HandleKeyButtonReleased^ _value)
			{
				bool empty = mDelegateKeyButtonReleased == nullptr;

				mDelegateKeyButtonReleased += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeyButtonReleased +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::KeyCode > *>(
								new Delegate2< HandleKeyButtonReleased^ ,
								MyGUI::Widget * ,
								MyGUI::KeyCode >(mDelegateKeyButtonReleased) );
			}
			void remove(HandleKeyButtonReleased^ _value)
			{
				mDelegateKeyButtonReleased -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateKeyButtonReleased == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeyButtonReleased -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::KeyCode > *>(
								new Delegate2< HandleKeyButtonReleased^ ,
									MyGUI::Widget * ,
									MyGUI::KeyCode >(mDelegateKeyButtonReleased) );
			}
		}
	private:
		HandleKeyButtonReleased^ mDelegateKeyButtonReleased;
	public:
		delegate void HandleKeyButtonPressed(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<MyGUI::KeyCode>::Type _key ,
			Convert<unsigned int>::Type _char );
		event HandleKeyButtonPressed^ EventKeyButtonPressed
		{
			void add(HandleKeyButtonPressed^ _value)
			{
				bool empty = mDelegateKeyButtonPressed == nullptr;

				mDelegateKeyButtonPressed += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeyButtonPressed +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::Widget * ,
							MyGUI::KeyCode ,
							unsigned int > *>(
								new Delegate3< HandleKeyButtonPressed^ ,
								MyGUI::Widget * ,
								MyGUI::KeyCode ,
								unsigned int >(mDelegateKeyButtonPressed) );
			}
			void remove(HandleKeyButtonPressed^ _value)
			{
				mDelegateKeyButtonPressed -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateKeyButtonPressed == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventKeyButtonPressed -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::Widget * ,
							MyGUI::KeyCode ,
							unsigned int > *>(
								new Delegate3< HandleKeyButtonPressed^ ,
									MyGUI::Widget * ,
									MyGUI::KeyCode ,
									unsigned int >(mDelegateKeyButtonPressed) );
			}
		}
	private:
		HandleKeyButtonPressed^ mDelegateKeyButtonPressed;
	public:
		delegate void HandleKeySetFocus(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _old );
		event HandleKeySetFocus^ EventKeySetFocus
		{
			void add(HandleKeySetFocus^ _value)
			{
				bool empty = mDelegateKeySetFocus == nullptr;

				mDelegateKeySetFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeySetFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleKeySetFocus^ ,
								MyGUI::Widget * ,
								MyGUI::Widget * >(mDelegateKeySetFocus) );
			}
			void remove(HandleKeySetFocus^ _value)
			{
				mDelegateKeySetFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateKeySetFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeySetFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleKeySetFocus^ ,
									MyGUI::Widget * ,
									MyGUI::Widget * >(mDelegateKeySetFocus) );
			}
		}
	private:
		HandleKeySetFocus^ mDelegateKeySetFocus;
	public:
		delegate void HandleKeyLostFocus(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _new );
		event HandleKeyLostFocus^ EventKeyLostFocus
		{
			void add(HandleKeyLostFocus^ _value)
			{
				bool empty = mDelegateKeyLostFocus == nullptr;

				mDelegateKeyLostFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeyLostFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleKeyLostFocus^ ,
								MyGUI::Widget * ,
								MyGUI::Widget * >(mDelegateKeyLostFocus) );
			}
			void remove(HandleKeyLostFocus^ _value)
			{
				mDelegateKeyLostFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateKeyLostFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventKeyLostFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleKeyLostFocus^ ,
									MyGUI::Widget * ,
									MyGUI::Widget * >(mDelegateKeyLostFocus) );
			}
		}
	private:
		HandleKeyLostFocus^ mDelegateKeyLostFocus;
	public:
		delegate void HandleMouseButtonDoubleClick(
			Convert<MyGUI::Widget *>::Type _sender );
		event HandleMouseButtonDoubleClick^ EventMouseButtonDoubleClick
		{
			void add(HandleMouseButtonDoubleClick^ _value)
			{
				bool empty = mDelegateMouseButtonDoubleClick == nullptr;

				mDelegateMouseButtonDoubleClick += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick +=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Widget * > *>(
								new Delegate1< HandleMouseButtonDoubleClick^ ,
								MyGUI::Widget * >(mDelegateMouseButtonDoubleClick) );
			}
			void remove(HandleMouseButtonDoubleClick^ _value)
			{
				mDelegateMouseButtonDoubleClick -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseButtonDoubleClick == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonDoubleClick -=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Widget * > *>(
								new Delegate1< HandleMouseButtonDoubleClick^ ,
									MyGUI::Widget * >(mDelegateMouseButtonDoubleClick) );
			}
		}
	private:
		HandleMouseButtonDoubleClick^ mDelegateMouseButtonDoubleClick;
	public:
		delegate void HandleMouseButtonClick(
			Convert<MyGUI::Widget *>::Type _sender );
		event HandleMouseButtonClick^ EventMouseButtonClick
		{
			void add(HandleMouseButtonClick^ _value)
			{
				bool empty = mDelegateMouseButtonClick == nullptr;

				mDelegateMouseButtonClick += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonClick +=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Widget * > *>(
								new Delegate1< HandleMouseButtonClick^ ,
								MyGUI::Widget * >(mDelegateMouseButtonClick) );
			}
			void remove(HandleMouseButtonClick^ _value)
			{
				mDelegateMouseButtonClick -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseButtonClick == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonClick -=
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::Widget * > *>(
								new Delegate1< HandleMouseButtonClick^ ,
									MyGUI::Widget * >(mDelegateMouseButtonClick) );
			}
		}
	private:
		HandleMouseButtonClick^ mDelegateMouseButtonClick;
	public:
		delegate void HandleMouseButtonReleased(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<MyGUI::MouseButton>::Type _id );
		event HandleMouseButtonReleased^ EventMouseButtonReleased
		{
			void add(HandleMouseButtonReleased^ _value)
			{
				bool empty = mDelegateMouseButtonReleased == nullptr;

				mDelegateMouseButtonReleased += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonReleased +=
						static_cast< MyGUI::delegates::IDelegate4<
							MyGUI::Widget * ,
							int ,
							int ,
							MyGUI::MouseButton > *>(
								new Delegate4< HandleMouseButtonReleased^ ,
								MyGUI::Widget * ,
								int ,
								int ,
								MyGUI::MouseButton >(mDelegateMouseButtonReleased) );
			}
			void remove(HandleMouseButtonReleased^ _value)
			{
				mDelegateMouseButtonReleased -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseButtonReleased == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonReleased -=
						static_cast< MyGUI::delegates::IDelegate4<
							MyGUI::Widget * ,
							int ,
							int ,
							MyGUI::MouseButton > *>(
								new Delegate4< HandleMouseButtonReleased^ ,
									MyGUI::Widget * ,
									int ,
									int ,
									MyGUI::MouseButton >(mDelegateMouseButtonReleased) );
			}
		}
	private:
		HandleMouseButtonReleased^ mDelegateMouseButtonReleased;
	public:
		delegate void HandleMouseButtonPressed(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<MyGUI::MouseButton>::Type _id );
		event HandleMouseButtonPressed^ EventMouseButtonPressed
		{
			void add(HandleMouseButtonPressed^ _value)
			{
				bool empty = mDelegateMouseButtonPressed == nullptr;

				mDelegateMouseButtonPressed += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonPressed +=
						static_cast< MyGUI::delegates::IDelegate4<
							MyGUI::Widget * ,
							int ,
							int ,
							MyGUI::MouseButton > *>(
								new Delegate4< HandleMouseButtonPressed^ ,
								MyGUI::Widget * ,
								int ,
								int ,
								MyGUI::MouseButton >(mDelegateMouseButtonPressed) );
			}
			void remove(HandleMouseButtonPressed^ _value)
			{
				mDelegateMouseButtonPressed -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseButtonPressed == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseButtonPressed -=
						static_cast< MyGUI::delegates::IDelegate4<
							MyGUI::Widget * ,
							int ,
							int ,
							MyGUI::MouseButton > *>(
								new Delegate4< HandleMouseButtonPressed^ ,
									MyGUI::Widget * ,
									int ,
									int ,
									MyGUI::MouseButton >(mDelegateMouseButtonPressed) );
			}
		}
	private:
		HandleMouseButtonPressed^ mDelegateMouseButtonPressed;
	public:
		delegate void HandleMouseWheel(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<int>::Type _rel );
		event HandleMouseWheel^ EventMouseWheel
		{
			void add(HandleMouseWheel^ _value)
			{
				bool empty = mDelegateMouseWheel == nullptr;

				mDelegateMouseWheel += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseWheel +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							int > *>(
								new Delegate2< HandleMouseWheel^ ,
								MyGUI::Widget * ,
								int >(mDelegateMouseWheel) );
			}
			void remove(HandleMouseWheel^ _value)
			{
				mDelegateMouseWheel -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseWheel == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseWheel -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							int > *>(
								new Delegate2< HandleMouseWheel^ ,
									MyGUI::Widget * ,
									int >(mDelegateMouseWheel) );
			}
		}
	private:
		HandleMouseWheel^ mDelegateMouseWheel;
	public:
		delegate void HandleMouseMove(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<int>::Type _left ,
			Convert<int>::Type _top );
		event HandleMouseMove^ EventMouseMove
		{
			void add(HandleMouseMove^ _value)
			{
				bool empty = mDelegateMouseMove == nullptr;

				mDelegateMouseMove += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseMove +=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::Widget * ,
							int ,
							int > *>(
								new Delegate3< HandleMouseMove^ ,
								MyGUI::Widget * ,
								int ,
								int >(mDelegateMouseMove) );
			}
			void remove(HandleMouseMove^ _value)
			{
				mDelegateMouseMove -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseMove == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseMove -=
						static_cast< MyGUI::delegates::IDelegate3<
							MyGUI::Widget * ,
							int ,
							int > *>(
								new Delegate3< HandleMouseMove^ ,
									MyGUI::Widget * ,
									int ,
									int >(mDelegateMouseMove) );
			}
		}
	private:
		HandleMouseMove^ mDelegateMouseMove;
	public:
		delegate void HandleMouseDrag(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<MyGUI::MouseButton>::Type _value4 );
		event HandleMouseDrag^ EventMouseDrag
		{
			void add(HandleMouseDrag^ _value)
			{
				bool empty = mDelegateMouseDrag == nullptr;

				mDelegateMouseDrag += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseDrag +=
						static_cast< MyGUI::delegates::IDelegate4<
							MyGUI::Widget * ,
							int ,
							int ,
							MyGUI::MouseButton > *>(
								new Delegate4< HandleMouseDrag^ ,
								MyGUI::Widget * ,
								int ,
								int ,
								MyGUI::MouseButton >(mDelegateMouseDrag) );
			}
			void remove(HandleMouseDrag^ _value)
			{
				mDelegateMouseDrag -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseDrag == nullptr;
				
				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseDrag -=
						static_cast< MyGUI::delegates::IDelegate4<
							MyGUI::Widget * ,
							int ,
							int ,
							MyGUI::MouseButton > *>(
								new Delegate4< HandleMouseDrag^ ,
									MyGUI::Widget * ,
									int ,
									int ,
									MyGUI::MouseButton >(mDelegateMouseDrag) );
			}
		}
	private:
		HandleMouseDrag^ mDelegateMouseDrag;
	public:
		delegate void HandleMouseSetFocus(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _old );
		event HandleMouseSetFocus^ EventMouseSetFocus
		{
			void add(HandleMouseSetFocus^ _value)
			{
				bool empty = mDelegateMouseSetFocus == nullptr;

				mDelegateMouseSetFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseSetFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleMouseSetFocus^ ,
								MyGUI::Widget * ,
								MyGUI::Widget * >(mDelegateMouseSetFocus) );
			}
			void remove(HandleMouseSetFocus^ _value)
			{
				mDelegateMouseSetFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseSetFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseSetFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleMouseSetFocus^ ,
									MyGUI::Widget * ,
									MyGUI::Widget * >(mDelegateMouseSetFocus) );
			}
		}
	private:
		HandleMouseSetFocus^ mDelegateMouseSetFocus;
	public:
		delegate void HandleMouseLostFocus(
			Convert<MyGUI::Widget *>::Type _sender ,
			Convert<MyGUI::Widget *>::Type _new );
		event HandleMouseLostFocus^ EventMouseLostFocus
		{
			void add(HandleMouseLostFocus^ _value)
			{
				bool empty = mDelegateMouseLostFocus == nullptr;

				mDelegateMouseLostFocus += _value;
				MMYGUI_CHECK_NATIVE(mNative);

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseLostFocus +=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleMouseLostFocus^ ,
								MyGUI::Widget * ,
								MyGUI::Widget * >(mDelegateMouseLostFocus) );
			}
			void remove(HandleMouseLostFocus^ _value)
			{
				mDelegateMouseLostFocus -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				
				bool empty = mDelegateMouseLostFocus == nullptr;

				if (empty)
					static_cast<ThisType*>(mNative)->eventMouseLostFocus -=
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Widget * ,
							MyGUI::Widget * > *>(
								new Delegate2< HandleMouseLostFocus^ ,
									MyGUI::Widget * ,
									MyGUI::Widget * >(mDelegateMouseLostFocus) );
			}
		}
	private:
		HandleMouseLostFocus^ mDelegateMouseLostFocus;
	public:
		Convert<bool>::Type IsMaskPickInside(
			Convert<const MyGUI::types::TPoint < int > &>::Type _point ,
			Convert<const MyGUI::types::TCoord < int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To(
				static_cast<ThisType*>(mNative)->isMaskPickInside(
					Convert<const MyGUI::types::TPoint < int > &>::From(_point) ,
					Convert<const MyGUI::types::TCoord < int > &>::From(_coord) ) );
		}
	public:
		property Convert<bool>::Type RootKeyFocus
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getRootKeyFocus() );
			}
		}
	
	public:
		property Convert<bool>::Type RootMouseFocus
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getRootMouseFocus() );
			}
		}
	
	public:
		property Convert<bool>::Type InheritsPick
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getInheritsPick() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setInheritsPick( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type NeedMouseFocus
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedMouseFocus() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedMouseFocus( Convert<bool>::From(_value) );
			}
		}
	
	public:
		property Convert<bool>::Type NeedKeyFocus
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedKeyFocus() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedKeyFocus( Convert<bool>::From(_value) );
			}
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
	
		};

	} // namespace Managed
} // namespace MyGUI
