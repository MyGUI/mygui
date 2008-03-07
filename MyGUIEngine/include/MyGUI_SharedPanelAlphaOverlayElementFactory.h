/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_FACTORY_H__
#define __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "MyGUI_SharedPanelAlphaOverlayElement.h"

namespace MyGUI
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class _MyGUIExport SharedPanelAlphaOverlayElementFactory: public Ogre::OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName)
        {
            return new SharedPanelAlphaOverlayElement(instanceName);
        }
        /** See OverlayElementFactory */
        const Ogre::String& getTypeName(void) const
        {
            static Ogre::String name = "SharedPanelAlpha";
            return name;
        }
    };

} // namespace MyGUI

#endif // __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_FACTORY_H__
