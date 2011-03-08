/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef __WRAP_PANEL_H__
#define __WRAP_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class WrapPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( WrapPanel )

	public:
		WrapPanel();

	protected:
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);
		virtual void overrideArrange();

	private:
		void alignChildLine(size_t _startIndex, size_t _stopIndex, int _top, int _height);
	};

} // namespace MyGUI

#endif // __WRAP_PANEL_H__
