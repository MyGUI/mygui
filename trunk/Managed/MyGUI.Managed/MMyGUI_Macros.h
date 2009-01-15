/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

namespace MMyGUI
{

	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_DELEGATE1(name, type1) \
	public: \
		delegate void Handle##name(Convert<type1>::Type _value1); \
		event Handle##name^ ##name \
		{ \
			void add(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name += _value; \
				static_cast<ThisType*>(mNative)->event##name = new Delegate1<Handle##name^, type1>(mDelegate##name); \
			} \
			void remove(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name -= _value; \
				if (mDelegate##name != nullptr) { \
					static_cast<ThisType*>(mNative)->event##name = new Delegate1<Handle##name^, type1>(mDelegate##name); \
				} \
			} \
		} \
	private: \
		Handle##name^ mDelegate##name;

#define MMYGUI_DECLARE_DELEGATE2(name, type1, type2) \
	public: \
		delegate void Handle##name(Convert<type1>::Type _value1, Convert<type2>::Type _value2); \
		event Handle##name^ ##name \
		{ \
			void add(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name += _value; \
				static_cast<ThisType*>(mNative)->event##name = new Delegate2<Handle##name^, type1, type2>(mDelegate##name); \
			} \
			void remove(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name -= _value; \
				if (mDelegate##name != nullptr) { \
					static_cast<ThisType*>(mNative)->event##name = new Delegate2<Handle##name^, type1, type2>(mDelegate##name); \
				} \
			} \
		} \
	private: \
		Handle##name^ mDelegate##name;

#define MMYGUI_DECLARE_DELEGATE3(name, type1, type2, type3) \
	public: \
		delegate void Handle##name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, Convert<type3>::Type _value3); \
		event Handle##name^ ##name \
		{ \
			void add(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name += _value; \
				static_cast<ThisType*>(mNative)->event##name = new Delegate3<Handle##name^, type1, type2, type3>(mDelegate##name); \
			} \
			void remove(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name -= _value; \
				if (mDelegate##name != nullptr) { \
					static_cast<ThisType*>(mNative)->event##name = new Delegate3<Handle##name^, type1, type2, type3>(mDelegate##name); \
				} \
			} \
		} \
	private: \
		Handle##name^ mDelegate##name;

#define MMYGUI_DECLARE_DELEGATE4(name, type1, type2, type3, type4) \
	public: \
		delegate void Handle##name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, Convert<type3>::Type _value3, Convert<type4>::Type _value4); \
		event Handle##name^ ##name \
		{ \
			void add(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name += _value; \
				static_cast<ThisType*>(mNative)->event##name = new Delegate4<Handle##name^, type1, type2, type3, type4>(mDelegate##name); \
			} \
			void remove(Handle##name^ _value) \
			{ \
				static_cast<ThisType*>(mNative)->event##name = 0; \
				mDelegate##name -= _value; \
				if (mDelegate##name != nullptr) { \
					static_cast<ThisType*>(mNative)->event##name = new Delegate4<Handle##name^, type1, type2, type3, type4>(mDelegate##name); \
				} \
			} \
		} \
	private: \
		Handle##name^ mDelegate##name;





	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_ENUM(Type, Value) \
	static Type Value = Type(MyGUI::Type::Value);

#define MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(T) \
	static operator MyGUI::T& (T& obj) { return reinterpret_cast<MyGUI::T&>(obj); } \
	static operator const T& ( const MyGUI::T& obj) { return reinterpret_cast<const T&>(obj); } \
	static operator const T& ( const MyGUI::T* pobj) { return reinterpret_cast<const T&>(*pobj); }

#define MMYGUI_DECLARE_EQUALS(T) \
    static bool operator != ( T lvalue, T rvalue ) { return !(lvalue == rvalue); } \
	virtual bool Equals(T other) { return *this == other; }

	//------------------------------------------------------------------------------//
#define MMYGUI_CHECK_NATIVE(ptr) \
	if (ptr == nullptr) throw gcnew System::NullReferenceException();

	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_PROPERTY(name, type) \
	public: \
		property type name \
		{ \
			type get() \
			{ \
				MMYGUI_CHECK_NATIVE(mNative); \
				return static_cast<ThisType*>(mNative)->get##name(); \
			} \
			void set(type _value) \
			{ \
				MMYGUI_CHECK_NATIVE(mNative); \
				static_cast<ThisType*>(mNative)->set##name(_value); \
			} \
		}

#define MMYGUI_DECLARE_PROPERTY_IS(name, type) \
	public: \
		property type name \
		{ \
			type get() \
			{ \
				MMYGUI_CHECK_NATIVE(mNative); \
				return static_cast<ThisType*>(mNative)->is##name(); \
			} \
			void set(type _value) \
			{ \
				MMYGUI_CHECK_NATIVE(mNative); \
				static_cast<ThisType*>(mNative)->set##name(_value); \
			} \
		}


	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_METHOD_RET0(name, typeret) \
	public: \
		Convert<typeret>::Type name() \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return Convert<typeret>::To(static_cast<ThisType*>(mNative)->name()); \
		}

#define MMYGUI_DECLARE_METHOD_RET1(name, typeret, type1) \
	public: \
		Convert<typeret>::Type name(Convert<type1>::Type _value1) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return Convert<typeret>::To(static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1))); \
		}

#define MMYGUI_DECLARE_METHOD_RET2(name, typeret, type1, type2) \
	public: \
		Convert<typeret>::Type name(Convert<type1>::Type _value1, Convert<type2>::Type _value2) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return Convert<typeret>::To(static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2))); \
		}

#define MMYGUI_DECLARE_METHOD_RET3(name, typeret, type1, type2, type3) \
	public: \
		Convert<typeret>::Type name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, \
			Convert<type3>::Type _value3) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return Convert<typeret>::To(static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2), \
				Convert<type3>::From(_value3))); \
		}

#define MMYGUI_DECLARE_METHOD_RET4(name, typeret, type1, type2, type3, type4) \
	public: \
		Convert<typeret>::Type name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, \
			Convert<type3>::Type _value3, Convert<type4>::Type _value4) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return Convert<typeret>::To(static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2), \
				Convert<type3>::From(_value3), Convert<type4>::From(_value4))); \
		}

#define MMYGUI_DECLARE_METHOD_RET5(name, typeret, type1, type2, type3, type4, type5) \
	public: \
		Convert<typeret>::Type name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, \
			Convert<type3>::Type _value3, Convert<type4>::Type _value4, Convert<type5>::Type _value5) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return Convert<typeret>::To(static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2), \
				Convert<type3>::From(_value3), Convert<type4>::From(_value4), Convert<type5>::From(_value5))); \
		}


#define MMYGUI_DECLARE_METHOD0(name) \
	public: \
		void name() \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(); \
		}

#define MMYGUI_DECLARE_METHOD1(name, type1) \
	public: \
		void name(Convert<type1>::Type _value1) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1)); \
		}

#define MMYGUI_DECLARE_METHOD2(name, type1, type2) \
	public: \
		void name(Convert<type1>::Type _value1, Convert<type2>::Type _value2) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2)); \
		}

#define MMYGUI_DECLARE_METHOD3(name, type1, type2, type3) \
	public: \
		void name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, \
			Convert<type3>::Type _value3) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2), \
				Convert<type3>::From(_value3)); \
		}

#define MMYGUI_DECLARE_METHOD4(name, type1, type2, type3, type4) \
	public: \
		void name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, \
			Convert<type3>::Type _value3, Convert<type4>::Type _value4) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2), \
				Convert<type3>::From(_value3), Convert<type4>::From(_value4)); \
		}

#define MMYGUI_DECLARE_METHOD5(name, type1, type2, type3, type4, type5) \
	public: \
		void name(Convert<type1>::Type _value1, Convert<type2>::Type _value2, \
			Convert<type3>::Type _value3, Convert<type4>::Type _value4, Convert<type5>::Type _value5) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(Convert<type1>::From(_value1), Convert<type2>::From(_value2), \
				Convert<type3>::From(_value3), Convert<type4>::From(_value4), Convert<type5>::From(_value5)); \
		}



	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_BASE(Type) \
	private: \
		typedef MyGUI::Type ThisType; \
	public: \
		Type() : mNative(0), mIsWrap(true) { } \
	internal: \
		Type( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			CreateWidget(_parent,  _skin, _coord, _align, _layer, _name); \
		} \
		~Type() \
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
		void CreateWidget(Type^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			if (_parent == nullptr) { \
				mNative = MyGUI::Gui::getInstance().createWidgetT( \
					getClassTypeName(), \
					managed_to_utf8(_skin), \
					_coord, \
					_align, \
					managed_to_utf8(_layer), \
					managed_to_utf8(_name)); \
				mRoots.Add(this); \
			} \
			else \
			{ \
				mNative = _parent->mNative->createWidgetT( \
					getClassTypeName(), \
					managed_to_utf8(_skin), \
					_coord, \
					_align, \
					managed_to_utf8(_name)); \
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
				Type^ child = mChilds[0]; \
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
			child->CreateWidget(this, _skin, _coord, _align, "", _name); \
			return (WidgetType)child; \
		} \
		generic <typename WidgetType> where WidgetType : ref class \
		WidgetType CreateWidget(System::String^ _skin, IntCoord _coord, Align _align) \
		{ \
			Widget^ child = (Widget^)(System::Activator::CreateInstance<WidgetType>()); \
			child->CreateWidget(this, _skin, _coord, _align, "", ""); \
			return (WidgetType)child; \
		} \
	internal: \
		virtual const std::string& getClassTypeName() { return ThisType::getClassTypeName(); } \
	internal: \
		MyGUI::Type* mNative; \
	private: \
		bool mIsWrap; \
		Type^ mParent; \
		System::Collections::Generic::List<Type^> mChilds; \
		static System::Collections::Generic::List<Type^> mRoots;
		

#define MMYGUI_DECLARE_DERIVED(Type, BT) \
	private: \
		typedef MyGUI::Type ThisType; \
		typedef MyGUI::BT BaseType; \
	public: \
		Type() : BT() { } \
	internal: \
		Type( MyGUI::Type* _native ) : BT(_native) { } \
		Type( Widget^ _parent, System::String^ _skin, IntCoord _coord, MMyGUI::Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			CreateWidget(_parent,  _skin, _coord, _align, _layer, _name); \
		} \
	internal: \
		virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }



} // namespace MMyGUI
