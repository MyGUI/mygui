/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/
#ifndef __SCROLL_VIEW_PANEL_H__
#define __SCROLL_VIEW_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class ScrollViewPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( ScrollViewPanel )

	public:
		ScrollViewPanel();

	protected:
		virtual void shutdownOverride();
		virtual void initialiseOverride();

		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);
		virtual void overrideArrange();

	private:
		ScrollView* mScrollView;
	};

} // namespace MyGUI

#endif // __SCROLL_VIEW_PANEL_H__
