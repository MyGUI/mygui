/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f305d8a5_c091_4f7b_9744_8bec205b02b4_
#define _f305d8a5_c091_4f7b_9744_8bec205b02b4_

#include "IFactoryItem.h"

namespace tools
{
	class Control;
	class MYGUI_EXPORT_DLL IControlController :
		public components::IFactoryItem
	{
	public:
		IControlController() { }
		virtual ~IControlController() { }

		virtual void setTarget(Control* _control) = 0;

		virtual void activate() = 0;
		virtual void deactivate() = 0;
	};
}

#endif
