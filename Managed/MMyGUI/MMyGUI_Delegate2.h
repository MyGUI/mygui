/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_DELEGATE_2_H__
#define __MMYGUI_DELEGATE_2_H__

#include <MyGUI.h>
#include <gcroot.h>

#include "MMyGUI_Marshaling.h"
#include "MMyGUI_Common.h"

namespace MMyGUI
{

	template <typename MD, typename T1, typename T2>
	class Delegate2 : public MyGUI::delegates::IDelegate2<T1, T2>
	{
	public:
		Delegate2 (MD _delegate) : mDelegate(_delegate) { }

		virtual bool isType( const std::type_info & _type) { return typeid( Delegate2<MD, T1, T2> ) == _type; }

		virtual void invoke( T1 p1, T2 p2 )
		{
			((MD)mDelegate)(ConvertToType<T1>::ConvertToValue(p1), ConvertToType<T2>::ConvertToValue(p2));
		}

		virtual bool compare(  MyGUI::delegates::IDelegate2<T1, T2>  * _delegate)
		{
			if (0 == _delegate || false == _delegate->isType(typeid(Delegate2<MD, T1, T2>)) ) return false;
			Delegate2<MD, T1, T2> * cast = static_cast<Delegate2<MD, T1, T2> *>(_delegate);
			return ((MD)cast->mDelegate) == ((MD)mDelegate);
		}

	private:
		gcroot<MD> mDelegate;
	};

#define MMYGUI_DECLARE_DELEGATE2(name, type1, type2) \
	public: \
		delegate void Handle##name(ConvertToType<type1>::Type _value1, ConvertToType<type2>::Type _value2); \
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

} // namespace MMyGUI

#endif // __MMYGUI_DELEGATE_2_H__
