/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef __STACK_PANEL_H__
#define __STACK_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class StackPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( StackPanel )

	public:
		StackPanel();

	protected:
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);
		virtual void overrideArrange();
	};

} // namespace MyGUI

#endif // __STACK_PANEL_H__
