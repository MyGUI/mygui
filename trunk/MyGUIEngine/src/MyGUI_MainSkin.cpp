/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_MainSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	MainSkin::MainSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent)
	{
		mAlign = Align::Stretch;
		setCoord(IntCoord(0, 0, _parent->getWidth(), _parent->getHeight()));
	}

	MainSkin::~MainSkin()
	{
	}

	StateInfo * MainSkin::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		return SubSkin::createStateData(_node, _root, _version);
	}

} // namespace MyGUI
