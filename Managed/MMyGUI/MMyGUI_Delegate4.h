/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_DELEGATE_4_H__
#define __MMYGUI_DELEGATE_4_H__

#include <MyGUI.h>
#include <gcroot.h>

#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

	template <typename MD, typename T1, typename T2, typename T3, typename T4>
	class Delegate4 : public MyGUI::delegates::IDelegate4<T1, T2, T3, T4>
	{
	public:
		Delegate4 (MD _delegate) : mDelegate(_delegate) { }

		virtual bool isType( const std::type_info & _type) { return typeid( Delegate4<MD, T1, T2, T3, T4> ) == _type; }

		virtual void invoke( T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			((MD)mDelegate)(ConvertValue(p1), ConvertValue(p2), ConvertValue(p3), ConvertValue(p4));
		}

		virtual bool compare(  MyGUI::delegates::IDelegate4<T1, T2, T3, T4>  * _delegate)
		{
			if (0 == _delegate || false == _delegate->isType(typeid(Delegate4<MD, T1, T2, T3, T4>)) ) return false;
			Delegate4<MD, T1, T2, T3, T4> * cast = static_cast<Delegate4<MD, T1, T2, T3, T4> *>(_delegate);
			return ((MD)cast->mDelegate) == ((MD)mDelegate);
		}

	private:
		gcroot<MD> mDelegate;
	};

#define MYGUI_DECLARE_DELEGATE4(name, type1, type2, type3, type4) \
	public: \
		delegate void Handle##name(ConvertType<type1>::Type _value1, ConvertType<type2>::Type _value2, ConvertType<type3>::Type _value3, ConvertType<type4>::Type _value4); \
		event Handle##name^ ##name \
		{ \
			void add(Handle##name^ _value) \
			{ \
				mNative->event##name = 0; \
				mDelegate##name += _value; \
				mNative->event##name = new Delegate4<Handle##name^, type1, type2, type3, type4>(mDelegate##name); \
			} \
			void remove(Handle##name^ _value) \
			{ \
				mNative->event##name = 0; \
				mDelegate##name -= _value; \
				if (mDelegate##name != nullptr) { \
					mNative->event##name = new Delegate4<Handle##name^, type1, type2, type3, type4>(mDelegate##name); \
				} \
			} \
		} \
	private: \
		Handle##name^ mDelegate##name;

} // namespace MMyGUI

#endif // __MMYGUI_DELEGATE_4_H__
