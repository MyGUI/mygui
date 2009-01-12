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

	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_PROPERTY(name, type) \
	public: \
		property type name \
		{ \
			type get() { return static_cast<ThisType*>(mNative)->get##name(); } \
			void set(type _value) { static_cast<ThisType*>(mNative)->set##name(_value); } \
		}

#define MMYGUI_DECLARE_PROPERTY_IS(name, type) \
	public: \
		property type name \
		{ \
			type get() { return static_cast<ThisType*>(mNative)->is##name(); } \
			void set(type _value) { static_cast<ThisType*>(mNative)->set##name(_value); } \
		}


	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_METHOD0(typeret, name) \
	public: \
		typeret name() \
		{ \
			return static_cast<ThisType*>(mNative)->name(); \
		}

#define MMYGUI_DECLARE_METHOD1(typeret, name, type1) \
	public: \
		typeret name(ConvertToType<type1>::Type _value1) \
		{ \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1)); \
		}

#define MMYGUI_DECLARE_METHOD2(typeret, name, type1, type2) \
	public: \
		typeret name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2) \
		{ \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2)); \
		}

#define MMYGUI_DECLARE_METHOD3(typeret, name, type1, type2, type3) \
	public: \
		typeret name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3) \
		{ \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3)); \
		}

#define MMYGUI_DECLARE_METHOD4(typeret, name, type1, type2, type3, type4) \
	public: \
		typeret name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3, ConvertToType<type4>::Type _value4) \
		{ \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3), ConvertToType<type4>::ConvertFromValue(_value4)); \
		}

#define MMYGUI_DECLARE_METHOD5(typeret, name, type1, type2, type3, type4, type5) \
	public: \
		typeret name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2, \
			ConvertToType<type3>::Type _value3, ConvertToType<type4>::Type _value4, ConvertToType<type5>::Type _value5) \
		{ \
			return static_cast<ThisType*>(mNative)->name(ConvertToType<type1>::ConvertFromValue(_value1), ConvertToType<type2>::ConvertFromValue(_value2), \
				ConvertToType<type3>::ConvertFromValue(_value3), ConvertToType<type4>::ConvertFromValue(_value4), ConvertToType<type5>::ConvertFromValue(_value5)); \
		}


	//------------------------------------------------------------------------------//
#define MMYGUI_DECLARE_BASE(T) \
	private: \
		typedef MyGUI::T ThisType; \
	public: \
		T() : mNative(0) { } \
		T( MyGUI::T* _native ) : mNative(_native) { } \
		T( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, ""); \
		} \
		T( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, _name); \
		} \
		T( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), _parent,  _skin, _coord, _align, "", ""); \
		} \
		T( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _name) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), _parent,  _skin, _coord, _align, "", _name); \
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
	protected: \
		void CreateWidget(const std::string& _type, T^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			if (_parent == nullptr) { \
				mNative = MyGUI::Gui::getInstance().createWidgetT( \
					_type, \
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
					_type, \
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
	protected: \
		MyGUI::T* mNative; \
		T^ mParent; \
		System::Collections::Generic::List<T^> mChilds; \
		static System::Collections::Generic::List<T^> mRoots;
		

#define MMYGUI_DECLARE_DERIVED(T, BT) \
	private: \
		typedef MyGUI::T ThisType; \
		typedef MyGUI::BT BaseType; \
	public: \
		T() : BT() { } \
		T( MyGUI::T* _native ) : BT(_native) { } \
		T( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, ""); \
		} \
		T( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, _name); \
		} \
		T( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), _parent,  _skin, _coord, _align, "", ""); \
		} \
		T( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _name) \
		{ \
			CreateWidget(MyGUI::T::getClassTypeName(), _parent,  _skin, _coord, _align, "", _name); \
		}

} // namespace MMyGUI

#endif // __MMYGUI_COMMON_H__
