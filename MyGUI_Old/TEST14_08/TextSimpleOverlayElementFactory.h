#pragma once

#include <OgreOverlayElementFactory.h>
#include "TextSimpleOverlayElement.h"

namespace widget
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class TextSimpleOverlayElementFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new TextSimpleOverlayElement(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "TextSimple";
            return name;
        }
    };

} // namespace widget