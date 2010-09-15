/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_EditBox.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class ComboBox : public EditBox
		{

		private:
			typedef MyGUI::ComboBox ThisType;

		public:
			ComboBox() : EditBox() { }

		internal:
			ComboBox( MyGUI::ComboBox* _native ) : EditBox(_native) { }
			ComboBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew ComboBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		delegate void HandleComboChangePosition(
			Convert<MyGUI::ComboBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleComboChangePosition^ EventComboChangePosition
		{
			void add(HandleComboChangePosition^ _value)
			{
				mDelegateComboChangePosition += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventComboChangePosition =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::ComboBox * ,
						size_t > *>(
							new Delegate2< HandleComboChangePosition^ ,
							MyGUI::ComboBox * ,
							size_t >(mDelegateComboChangePosition) );
			}
			void remove(HandleComboChangePosition^ _value)
			{
				mDelegateComboChangePosition -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateComboChangePosition == nullptr)
					static_cast<ThisType*>(mNative)->eventComboChangePosition = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventComboChangePosition =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ComboBox * ,
							size_t > *>(
								new Delegate2< HandleComboChangePosition^ ,
									MyGUI::ComboBox * ,
									size_t >(mDelegateComboChangePosition) );
			}
		}
	private:
		HandleComboChangePosition^ mDelegateComboChangePosition;



   	public:
		delegate void HandleComboAccept(
			Convert<MyGUI::ComboBox *>::Type _sender ,
			Convert<size_t>::Type _index );
		event HandleComboAccept^ EventComboAccept
		{
			void add(HandleComboAccept^ _value)
			{
				mDelegateComboAccept += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventComboAccept =
					static_cast< MyGUI::delegates::IDelegate2<
						MyGUI::ComboBox * ,
						size_t > *>(
							new Delegate2< HandleComboAccept^ ,
							MyGUI::ComboBox * ,
							size_t >(mDelegateComboAccept) );
			}
			void remove(HandleComboAccept^ _value)
			{
				mDelegateComboAccept -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateComboAccept == nullptr)
					static_cast<ThisType*>(mNative)->eventComboAccept = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventComboAccept =
						static_cast< MyGUI::delegates::IDelegate2<
							MyGUI::ComboBox * ,
							size_t > *>(
								new Delegate2< HandleComboAccept^ ,
									MyGUI::ComboBox * ,
									size_t >(mDelegateComboAccept) );
			}
		}
	private:
		HandleComboAccept^ mDelegateComboAccept;



		};

	} // namespace Managed
} // namespace MyGUI
