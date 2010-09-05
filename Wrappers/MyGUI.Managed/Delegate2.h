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

		template <typename MD, typename T1, typename T2>
		class Delegate2 : public MyGUI::delegates::IDelegate2<T1, T2>
		{
		public:
			Delegate2 (MD _delegate) : mDelegate(_delegate) { }

			virtual bool isType( const std::type_info& _type)
			{
				return typeid( Delegate2<MD, T1, T2> ) == _type;
			}

			virtual void invoke( T1 p1, T2 p2 )
			{
				((MD)mDelegate)(Convert<T1>::To(p1), Convert<T2>::To(p2));
			}

			virtual bool compare(  MyGUI::delegates::IDelegate2<T1, T2>  * _delegate) const
			{
				if (nullptr == _delegate || false == _delegate->isType(typeid(Delegate2<MD, T1, T2>)) ) return false;
				Delegate2<MD, T1, T2> * cast = static_cast<Delegate2<MD, T1, T2> *>(_delegate);
				return ((MD)cast->mDelegate) == ((MD)mDelegate);
			}

		private:
			gcroot<MD> mDelegate;
		};

	} // namespace Managed
} // namespace MyGUI
