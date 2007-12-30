/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_EDIT_OVERLAY_ELEMENT_FACTORY_H__
#define __MYGUI_TEXT_EDIT_OVERLAY_ELEMENT_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "MyGUI_TextEditOverlayElement.h"

namespace MyGUI
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
	class _MyGUIExport TextEditOverlayElementFactory: public Ogre::OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new TextEditOverlayElement(instanceName);
        }
        /** See OverlayElementFactory */
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "TextEdit";
            return name;
        }
    };

} // namespace MyGUI

#endif // __MYGUI_TEXT_EDIT_OVERLAY_ELEMENT_FACTORY_H__
