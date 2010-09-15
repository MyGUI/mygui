/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Window.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class MessageBox : public Window
		{

		private:
			typedef MyGUI::Message ThisType;

		public:
			MessageBox() : Window() { }

		internal:
			MessageBox( MyGUI::Message* _native ) : Window(_native) { }
			MessageBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew MessageBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		delegate void HandleMessageBoxResult(
			Convert<MyGUI::Message *>::Type _sender ,
			Convert<MyGUI::MessageBoxStyle>::Type _result );
		event HandleMessageBoxResult^ EventMessageBoxResult
		{
			void add(HandleMessageBoxResult^ _value)
			{
				mDelegateMessageBoxResult += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMessageBoxResult =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::Message * ,
						MyGUI::MessageBoxStyle > *>(
							new Delegate2< HandleMessageBoxResult^ ,
							MyGUI::Message * ,
							MyGUI::MessageBoxStyle >(mDelegateMessageBoxResult) );
			}
			void remove(HandleMessageBoxResult^ _value)
			{
				mDelegateMessageBoxResult -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMessageBoxResult == nullptr)
					static_cast<ThisType*>(mNative)->eventMessageBoxResult = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMessageBoxResult =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::Message * ,
							MyGUI::MessageBoxStyle > *>(
								new Delegate2< HandleMessageBoxResult^ ,
									MyGUI::Message * ,
									MyGUI::MessageBoxStyle >(mDelegateMessageBoxResult) );
			}
		}
	private:
		HandleMessageBoxResult^ mDelegateMessageBoxResult;



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



		};

	} // namespace Managed
} // namespace MyGUI
