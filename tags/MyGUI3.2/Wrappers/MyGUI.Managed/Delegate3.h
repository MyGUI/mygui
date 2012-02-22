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

		template <typename MD, typename T1, typename T2, typename T3>
		class Delegate3 : public MyGUI::delegates::IDelegate3<T1, T2, T3>
		{
		public:
			Delegate3 (MD _delegate) : mDelegate(_delegate) { }

			virtual bool isType( const std::type_info& _type)
			{
				return typeid( Delegate3<MD, T1, T2, T3> ) == _type;
			}

			virtual void invoke( T1 p1, T2 p2, T3 p3 )
			{
				((MD)mDelegate)(Convert<T1>::To(p1), Convert<T2>::To(p2), Convert<T3>::To(p3));
			}

			virtual bool compare(  MyGUI::delegates::IDelegate3<T1, T2, T3>  * _delegate) const
			{
				if (nullptr == _delegate || false == _delegate->isType(typeid(Delegate3<MD, T1, T2, T3>)) ) return false;
				Delegate3<MD, T1, T2, T3> * cast = static_cast<Delegate3<MD, T1, T2, T3> *>(_delegate);
				return ((MD)cast->mDelegate) == ((MD)mDelegate);
			}

		protected:
			gcroot<MD> mDelegate;
		};

	} // namespace Managed
} // namespace MyGUI
