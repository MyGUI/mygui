#pragma once

#include <OgreOverlayElementFactory.h>
#include "PanelAlphaOverlayElement.h"

namespace widget
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class PanelAlphaOverlayElementFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new PanelAlphaOverlayElement(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "PanelAlpha";
            return name;
        }
    };

} // namespace widget