/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

namespace MMyGUI
{

#define MMYGUI_DECLARE_EQUALS(T) \
    static bool operator != ( T lvalue, T rvalue ) { return !(lvalue == rvalue); } \
	virtual bool Equals(T other) { return *this == other; }

	//------------------------------------------------------------------------------//
#define MMYGUI_CHECK_NATIVE(ptr) \
	if (ptr == nullptr) throw gcnew System::NullReferenceException();

	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_BASE(TYPE) \
	private: \
		typedef MyGUI::TYPE ThisType; \
	public: \
		TYPE() : mNative(0), mIsWrap(true) { } \
	internal: \
		TYPE( Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name ) \
		{ \
			CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name); \
		} \
		~TYPE() \
		{ \
			if (mNative != 0) \
			{ \
				DestroyChilds(); \
				if (mParent == nullptr) \
				{ \
					mRoots.Remove(this); \
				} \
				else \
				{ \
					mParent->mChilds.Remove(this); \
					mParent = nullptr; \
				} \
				if ( ! mIsWrap ) \
					MyGUI::WidgetManager::getInstance().destroyWidget(mNative); \
				mNative = 0; \
			} \
		} \
	internal: \
		void CreateWidget( Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name ) \
		{ \
			if (_parent == nullptr) { \
				mNative = MyGUI::Gui::getInstance().createWidgetT( \
					getClassTypeName(), \
					_skin, \
					_coord, \
					_align, \
					_layer, \
					_name); \
				mRoots.Add(this); \
			} \
			else \
			{ \
				mNative = _parent->mNative->createWidgetT( \
					getClassTypeName(), \
					_skin, \
					_coord, \
					_align, \
					_name); \
				mParent = _parent; \
				mParent->mChilds.Add(this); \
			} \
			WidgetHolder sender = this; \
			mNative->setUserData(sender); \
			mIsWrap = false; \
		} \
		void DestroyChilds() \
		{ \
			while (mChilds.Count > 0) { \
				TYPE^ child = mChilds[0]; \
				delete child; \
				child = nullptr; \
			} \
		} \
		MyGUI::Widget* GetNativePtr() { return mNative; } \
		Widget( MyGUI::Widget* _native ) \
		{ \
			if (_native == nullptr) return; \
			mNative = _native; \
			mParent = getMangedParent(mNative); \
			if (mParent == nullptr) mRoots.Add(this); \
			else mParent->mChilds.Add(this); \
			WidgetHolder sender = this; \
			mNative->setUserData(sender); \
			mIsWrap = true; \
		} \
		Widget^ getMangedParent(MyGUI::Widget* _widget) \
		{ \
			MyGUI::Widget* parent = _widget->getParent(); \
			while (parent != nullptr) \
			{ \
				WidgetHolder * obj = parent->getUserData< WidgetHolder >(false); \
				if (obj != nullptr) return obj->toObject(); \
				parent = parent->getParent(); \
			} \
			return nullptr; \
		} \
	public: \
		generic <typename WidgetType> where WidgetType : ref class \
		WidgetType CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _name) \
		{ \
			Widget^ child = (Widget^)(System::Activator::CreateInstance<WidgetType>()); \
			child->CreateWidget(this, MyGUI::WidgetStyle::Child, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), "", Convert<const std::string&>::From(_name)); \
			return (WidgetType)child; \
		} \
		generic <typename WidgetType> where WidgetType : ref class \
		WidgetType CreateWidget(System::String^ _skin, IntCoord _coord, Align _align) \
		{ \
			Widget^ child = (Widget^)(System::Activator::CreateInstance<WidgetType>()); \
			child->CreateWidget(this, MyGUI::WidgetStyle::Child, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), "", ""); \
			return (WidgetType)child; \
		} \
		generic <typename WidgetType> where WidgetType : ref class \
		WidgetType CreateWidget(WidgetStyle _style, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			Widget^ child = (Widget^)(System::Activator::CreateInstance<WidgetType>()); \
			child->CreateWidget(this, Convert<MyGUI::WidgetStyle>::From(_style), Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name)); \
			return (WidgetType)child; \
		} \
	internal: \
		virtual const std::string& getClassTypeName() { return ThisType::getClassTypeName(); } \
	internal: \
		MyGUI::TYPE* mNative; \
	private: \
		bool mIsWrap; \
		TYPE^ mParent; \
		System::Collections::Generic::List<TYPE^> mChilds; \
		static System::Collections::Generic::List<TYPE^> mRoots; \
	internal: \
		static Widget^ WidgetCreator(Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name) \
		{ \
			return gcnew TYPE(_parent, _style, _skin, _coord, _align, _layer, _name); \
		}
		

#define MMYGUI_DECLARE_DERIVED(NAME, TYPE, BASE) \
	private: \
		typedef MyGUI::TYPE ThisType; \
	public: \
		NAME() : BASE() { } \
	internal: \
		NAME( MyGUI::TYPE* _native ) : BASE(_native) { } \
		NAME( Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name ) \
		{ \
			CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name); \
		} \
	internal: \
		virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); } \
		static Widget^ WidgetCreator(Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name) \
		{ \
			return gcnew NAME(_parent, _style, _skin, _coord, _align, _layer, _name); \
		}


} // namespace MMyGUI
