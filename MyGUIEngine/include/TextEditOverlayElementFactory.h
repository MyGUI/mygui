#ifndef __TEXT_EDIT_OVERLAY_ELEMENT_FACTORY_H__
#define __TEXT_EDIT_OVERLAY_ELEMENT_FACTORY_H__

#include "Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "TextEditOverlayElement.h"

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

#endif // __TEXT_EDIT_OVERLAY_ELEMENT_FACTORY_H__
