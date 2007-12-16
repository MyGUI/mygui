/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_ComboBox.h"

namespace MyGUI
{

	ComboBox::ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Edit(_coord, _align, _info, _parent, _name)
	{
		// запомием размер скина
		IntSize size = _info->getSize();

		// парсим свойства
		const MapString & param = _info->getParams();

		// парсим конечную кнопку
		mButton = parseSubWidget(param, "Button", "SkinDrop", "OffsetDrop", "AlignDrop", size);
		MYGUI_ASSERT(null != mButton);
	}

} // namespace MyGUI