/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_MainSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT(MainSkin, SubSkin);

	MainSkin::MainSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent)
	{
		mAlign = Align::Stretch;
		setPosition(IntCoord(0, 0, _parent->getWidth(), _parent->getHeight()));
	}

	MainSkin::~MainSkin()
	{
	}

	StateInfo * MainSkin::createStateData(xml::xmlNodePtr _node, xml::xmlNodePtr _root)
	{
		return SubSkin::createStateData(_node, _root);
	}

} // namespace MyGUI
