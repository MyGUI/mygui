/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_DELEGATE_1_H__
#define __MMYGUI_DELEGATE_1_H__

#include <MyGUI.h>
#include <gcroot.h>

#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

	template <typename MD, typename T1>
	class Delegate1 : public MyGUI::delegates::IDelegate1<T1>
	{
	public:
		Delegate1 (MD _delegate) : mDelegate(_delegate) { }

		virtual bool isType( const std::type_info & _type) { return typeid( Delegate1<MD, T1> ) == _type; }

		virtual void invoke( T1 p1 )
		{
			((MD)mDelegate)(ConvertToType<T1>::ConvertToValue(p1));
		}

		virtual bool compare(  MyGUI::delegates::IDelegate1<T1>  * _delegate)
		{
			if (0 == _delegate || false == _delegate->isType(typeid(Delegate1<MD, T1>)) ) return false;
			Delegate1<MD, T1> * cast = static_cast<Delegate1<MD, T1> *>(_delegate);
			return ((MD)cast->mDelegate) == ((MD)mDelegate);
		}

	private:
		gcroot<MD> mDelegate;
	};

#define MMYGUI_DECLARE_DELEGATE1(name, type1) \
	public: \
		delegate void Handle##name(ConvertToType<type1>::Type _value1); \
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

} // namespace MMyGUI

#endif // __MMYGUI_DELEGATE_1_H__
