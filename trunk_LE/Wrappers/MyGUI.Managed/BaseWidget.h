/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Config.h"
#include "NativePtrHolder.h"
#include "WidgetHolder.h"
#include "Types.h"
#include "Delegates.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class BaseWidget abstract
		{

		public:
			BaseWidget() : mNative(0), mIsWrap(true) { }

		internal:
			BaseWidget( MyGUI::Widget* _native )
			{
				if (_native == nullptr) return;
				mNative = _native;
				mParent = getMangedParent(mNative);
				if (mParent == nullptr) mRoots.Add(this);
				else mParent->mChilds.Add(this);
				WidgetHolder sender = this;
				mNative->setUserData(sender);
				mIsWrap = true;
			}

			~BaseWidget()
			{
				if (mNative != 0)
				{
					DestroyChilds();
					if (mParent == nullptr)
					{
						mRoots.Remove(this);
					}
					else
					{
						mParent->mChilds.Remove(this);
						mParent = nullptr;
					}
					if ( ! mIsWrap )
						MyGUI::WidgetManager::getInstance().destroyWidget(mNative);
					mNative = 0;
				}
			}

		internal:
			void CreateWidget( BaseWidget ^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				if (_parent == nullptr)
				{
					mNative = MyGUI::Gui::getInstance().createWidgetT(
						getClassTypeName(),
						_skin,
						_coord,
						_align,
						_layer,
						_name);
					mRoots.Add(this);
				}
				else
				{
					mNative = _parent->mNative->createWidgetT(
						getClassTypeName(),
						_skin,
						_coord,
						_align,
						_name);
					mParent = _parent;
					mParent->mChilds.Add(this);
				}
				WidgetHolder sender = this;
				mNative->setUserData(sender);
				mIsWrap = false;
			}

			void DestroyChilds()
			{
				while (mChilds.Count > 0)
				{
					BaseWidget ^ child = mChilds[0];
					delete child;
					child = nullptr;
				}
			}

			BaseWidget ^ getMangedParent(MyGUI::Widget* _widget)
			{
				MyGUI::Widget* parent = _widget->getParent();
				while (parent != nullptr)
				{
					WidgetHolder* obj = parent->getUserData< WidgetHolder >(false);
					if (obj != nullptr) return obj->toObject();
					parent = parent->getParent();
				}
				return nullptr;
			}

		public:
			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _name)
			{
				BaseWidget ^ child = (BaseWidget ^ )(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(this, MyGUI::WidgetStyle::Child, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), "", Convert<const std::string&>::From(_name));
				return (WidgetType)child;
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(System::String ^ _skin, IntCoord _coord, Align _align)
			{
				BaseWidget ^ child = (BaseWidget ^ )(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(this, MyGUI::WidgetStyle::Child, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), "", "");
				return (WidgetType)child;
			}

			generic <typename WidgetType> where WidgetType : ref class
			WidgetType CreateWidget(WidgetStyle _style, System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer, System::String ^ _name)
			{
				BaseWidget ^ child = (BaseWidget ^ )(System::Activator::CreateInstance<WidgetType>());
				child->CreateWidget(this, Convert<MyGUI::WidgetStyle>::From(_style), Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
				return (WidgetType)child;
			}

			BaseWidget ^ CreateWidgetT(System::Type ^ _type, WidgetStyle _style, System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer, System::String ^ _name)
			{
				System::Reflection::ConstructorInfo ^ ci = _type->GetConstructor(gcnew cli::array < System::Type ^ > (0));
				BaseWidget ^ child = (BaseWidget ^ )ci->Invoke(nullptr);
				child->CreateWidget(this, Convert<MyGUI::WidgetStyle>::From(_style), Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
				return child;
			}

		internal:
			virtual const std::string& getClassTypeName() = 0;
			MyGUI::Widget* GetNativePtr()
			{
				return mNative;
			}

		public:
			System::IntPtr GetNativeIntPtr()
			{
				return System::IntPtr(mNative);
			}

			property System::Object ^ UserData
			{
				System::Object ^ get( )
				{
					MMYGUI_CHECK_NATIVE(mNative);
					return mUserData;
				}
				void set(System::Object ^ _value)
				{
					MMYGUI_CHECK_NATIVE(mNative);
					mUserData = _value;
				}
			}

		internal:
			MyGUI::Widget* mNative;

		private:
			bool mIsWrap;
			BaseWidget ^ mParent;
			System::Collections::Generic::List < BaseWidget ^ > mChilds;
			System::Object ^ mUserData;
			static System::Collections::Generic::List < BaseWidget ^ > mRoots;

		};

	} // namespace Managed
} // namespace MyGUI
