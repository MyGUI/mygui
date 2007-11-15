#ifndef _TEXTEDITOVERLAYELEMENTFACTORY_H_
#define _TEXTEDITOVERLAYELEMENTFACTORY_H_

#include "Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "TextEditOverlayElement.h"

namespace MyGUI
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class _MyGUIExport TextEditOverlayElementFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new TextEditOverlayElement(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "TextEdit";
            return name;
        }
    };

} // namespace MyGUI

#endif
