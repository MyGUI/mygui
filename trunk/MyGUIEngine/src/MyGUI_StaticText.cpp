/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_StaticText.h"

namespace MyGUI
{
	Ogre::String StaticText::WidgetTypeName = "StaticText";

	StaticText::StaticText(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name)
	{
	}

} // namespace MyGUI
