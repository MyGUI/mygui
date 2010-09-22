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

		template <typename MD, typename T1, typename T2, typename T3, typename T4>
		class Delegate4 : public MyGUI::delegates::IDelegate4<T1, T2, T3, T4>
		{
		public:
			Delegate4 (MD _delegate) : mDelegate(_delegate) { }

			virtual bool isType( const std::type_info& _type)
			{
				return typeid( Delegate4<MD, T1, T2, T3, T4> ) == _type;
			}

			virtual void invoke( T1 p1, T2 p2, T3 p3, T4 p4 )
			{
				((MD)mDelegate)(Convert<T1>::To(p1), Convert<T2>::To(p2), Convert<T3>::To(p3), Convert<T4>::To(p4));
			}

			virtual bool compare(  MyGUI::delegates::IDelegate4<T1, T2, T3, T4>  * _delegate) const
			{
				if (nullptr == _delegate || false == _delegate->isType(typeid(Delegate4<MD, T1, T2, T3, T4>)) ) return false;
				Delegate4<MD, T1, T2, T3, T4> * cast = static_cast<Delegate4<MD, T1, T2, T3, T4> *>(_delegate);
				return ((MD)cast->mDelegate) == ((MD)mDelegate);
			}

		private:
			gcroot<MD> mDelegate;
		};

	} // namespace Managed
} // namespace MyGUI
