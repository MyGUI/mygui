/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef __WORD_WRAP_PANEL_H__
#define __WORD_WRAP_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class WordWrapPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( WordWrapPanel )

	public:
		WordWrapPanel();

	protected:
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);
	};

} // namespace MyGUI

#endif // __WORD_WRAP_PANEL_H__
