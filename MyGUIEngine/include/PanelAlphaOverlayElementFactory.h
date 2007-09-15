#ifndef _PANELALPHAOVERLAYELEMENTFACTORY_H_
#define _PANELALPHAOVERLAYELEMENTFACTORY_H_

#include "Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "PanelAlphaOverlayElement.h"

namespace MyGUI
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class _MyGUIExport PanelAlphaOverlayElementFactory: public OverlayElementFactory
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

} // namespace MyGUI

#endif
