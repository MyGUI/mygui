/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "MyGUI_DDContainer.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( DDContainer, Widget );

	DDContainer::DDContainer(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name)
	{
	}

} // namespace MyGUI
