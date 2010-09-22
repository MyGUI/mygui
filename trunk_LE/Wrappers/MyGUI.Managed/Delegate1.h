/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include <gcroot.h>

#include "Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		template <typename MD, typename T1>
		class Delegate1 : public MyGUI::delegates::IDelegate1<T1>
		{
		public:
			Delegate1 (MD _delegate) : mDelegate(_delegate) { }

			virtual bool isType( const std::type_info& _type)
			{
				return typeid( Delegate1<MD, T1> ) == _type;
			}

			virtual void invoke( T1 p1 )
			{
				((MD)mDelegate)(Convert<T1>::To(p1));
			}

			virtual bool compare(  MyGUI::delegates::IDelegate1<T1>  * _delegate) const
			{
				if (nullptr == _delegate || false == _delegate->isType(typeid(Delegate1<MD, T1>)) ) return false;
				Delegate1<MD, T1> * cast = static_cast<Delegate1<MD, T1> *>(_delegate);
				return ((MD)cast->mDelegate) == ((MD)mDelegate);
			}

		private:
			gcroot<MD> mDelegate;
		};

	} // namespace Managed
} // namespace MyGUI
