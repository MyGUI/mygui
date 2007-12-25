/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __UPDATE_WIDGET_FROM_ENTITY_H__
#define __UPDATE_WIDGET_FROM_ENTITY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"
#include "Ogre.h"

namespace helper
{

	void updateWidgetPosition(Ogre::Entity * _entity, Ogre::Camera * _camera, MyGUI::WidgetPtr _widget);

} // namespace helper

#endif // __UPDATE_WIDGET_FROM_ENTITY_H__