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
#include "Delegate3.h"

namespace MyGUI
{
	namespace Managed
	{

		template <typename MD, typename T1, typename T2, typename T3>
		class Delegate3_Ref2 : public Delegate3<MD, T1, T2, T3>
		{
		public:
			Delegate3_Ref2 (MD _delegate) : Delegate3<MD, T1, T2, T3>(_delegate) { }

			virtual void invoke( T1 p1, T2 p2, T3 p3 )
			{
				Widget ^ widget = nullptr;
				((MD)mDelegate)(Convert<T1>::To(p1), widget, Convert<T3>::To(p3));
				p2 = Convert<MyGUI::Widget*>::From(widget);
			}

		};

	} // namespace Managed
} // namespace MyGUI
