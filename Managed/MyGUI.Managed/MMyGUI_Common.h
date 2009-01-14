/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_COMMON_H__
#define __MMYGUI_COMMON_H__

namespace MMyGUI
{

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
		ConvertToType<typeret>::Type name() \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return ConvertToType<typeret>::ConvertToValue(static_cast<ThisType*>(mNative)->name()); \
		}

#define MMYGUI_DECLARE_METHOD_RET1(name, typeret, type1) \
	public: \
		ConvertToType<typeret>::Type name(ConvertToType<type1>::Type _value1) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return ConvertToType<typeret>::ConvertToValue(static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1))); \
		}

#define MMYGUI_DECLARE_METHOD_RET2(name, typeret, type1, type2) \
	public: \
		ConvertToType<typeret>::Type name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return ConvertToType<typeret>::ConvertToValue(static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2))); \
		}

#define MMYGUI_DECLARE_METHOD_RET3(name, typeret, type1, type2, type3) \
	public: \
		ConvertToType<typeret>::Type name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return ConvertToType<typeret>::ConvertToValue(static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3))); \
		}

#define MMYGUI_DECLARE_METHOD_RET4(name, typeret, type1, type2, type3, type4) \
	public: \
		ConvertToType<typeret>::Type name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3, ConvertToType<type4>::Type _value4) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return ConvertToType<typeret>::ConvertToValue(static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3), ConvertToType<type4>::ConvertFromValue(_value4))); \
		}

#define MMYGUI_DECLARE_METHOD_RET5(name, typeret, type1, type2, type3, type4, type5) \
	public: \
		ConvertToType<typeret>::Type name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3, ConvertToType<type4>::Type _value4, ConvertToType<type5>::Type _value5) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return ConvertToType<typeret>::ConvertToValue(static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3), ConvertToType<type4>::ConvertFromValue(_value4), ConvertToType<type5>::ConvertFromValue(_value5))); \
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
		void name(ConvertToType<type1>::Type _value1) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1)); \
		}

#define MMYGUI_DECLARE_METHOD2(name, type1, type2) \
	public: \
		void name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2)); \
		}

#define MMYGUI_DECLARE_METHOD3(name, type1, type2, type3) \
	public: \
		void name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3)); \
		}

#define MMYGUI_DECLARE_METHOD4(name, type1, type2, type3, type4) \
	public: \
		void name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3, ConvertToType<type4>::Type _value4) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3), ConvertToType<type4>::ConvertFromValue(_value4)); \
		}

#define MMYGUI_DECLARE_METHOD5(name, type1, type2, type3, type4, type5) \
	public: \
		void name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3, ConvertToType<type4>::Type _value4, ConvertToType<type5>::Type _value5) \
		{ \
			MMYGUI_CHECK_NATIVE(mNative); \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3), ConvertToType<type4>::ConvertFromValue(_value4), ConvertToType<type5>::ConvertFromValue(_value5)); \
		}



	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_BASE(T) \
	private: \
		typedef MyGUI::T ThisType; \
	public: \
		T() : mNative(0) { } \
	internal: \
		T( MyGUI::T* _native ) : mNative(_native) { } \
		T( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			CreateWidget(_parent,  _skin, _coord, _align, _layer, _name); \
		} \
		~T() \
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
				MyGUI::WidgetManager::getInstance().destroyWidget(mNative); \
				mNative = 0; \
			} \
		} \
	internal: \
		void CreateWidget(T^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
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
		} \
		void DestroyChilds() \
		{ \
			while (mChilds.Count > 0) { \
				T^ child = mChilds[0]; \
				delete child; \
				child = nullptr; \
			} \
		} \
	internal: \
		virtual const std::string& getClassTypeName() { return ThisType::getClassTypeName(); } \
	internal: \
		MyGUI::T* mNative; \
	private: \
		T^ mParent; \
		System::Collections::Generic::List<T^> mChilds; \
		static System::Collections::Generic::List<T^> mRoots;
		

#define MMYGUI_DECLARE_DERIVED(T, BT) \
	private: \
		typedef MyGUI::T ThisType; \
		typedef MyGUI::BT BaseType; \
	public: \
		T() : BT() { } \
	internal: \
		T( MyGUI::T* _native ) : BT(_native) { } \
		T( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			CreateWidget(_parent,  _skin, _coord, _align, _layer, _name); \
		} \
	internal: \
		virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

} // namespace MMyGUI

#endif // __MMYGUI_COMMON_H__
