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

		template <typename MD, typename T1, typename T2, typename T3, typename T4, typename T5>
		class Delegate5 : public MyGUI::delegates::IDelegate5<T1, T2, T3, T4, T5>
		{
		public:
			Delegate5 (MD _delegate) : mDelegate(_delegate) { }

			virtual bool isType( const std::type_info& _type)
			{
				return typeid( Delegate5<MD, T1, T2, T3, T4, T5> ) == _type;
			}

			virtual void invoke( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
			{
				((MD)mDelegate)(Convert<T1>::To(p1), Convert<T2>::To(p2), Convert<T3>::To(p3), Convert<T4>::To(p4), Convert<T5>::To(p5));
			}

			virtual bool compare(  MyGUI::delegates::IDelegate5<T1, T2, T3, T4, T5>  * _delegate) const
			{
				if (nullptr == _delegate || false == _delegate->isType(typeid(Delegate5<MD, T1, T2, T3, T4, T5>)) ) return false;
				Delegate5<MD, T1, T2, T3, T4, T5> * cast = static_cast<Delegate5<MD, T1, T2, T3, T4, T5> *>(_delegate);
				return ((MD)cast->mDelegate) == ((MD)mDelegate);
			}

		private:
			gcroot<MD> mDelegate;
		};

	} // namespace Managed
} // namespace MyGUI
