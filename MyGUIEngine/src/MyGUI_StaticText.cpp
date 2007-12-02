/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_StaticText.h"

namespace MyGUI
{

	StaticText::StaticText(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name)
	{
	}

} // namespace MyGUI