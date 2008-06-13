/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_SimpleText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	SimpleText::SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		EditText(_info, _parent)
	{
		mManualView = false;
	}

	SimpleText::~SimpleText()
	{
	}

	void SimpleText::setViewOffset(IntPoint _point)
	{
	}

} // namespace MyGUI
