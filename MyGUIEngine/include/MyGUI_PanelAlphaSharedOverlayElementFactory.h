#ifndef _PANELALPHASHAREDOVERLAYELEMENTFACTORY_H_
#define _PANELALPHASHAREDOVERLAYELEMENTFACTORY_H_

#include "Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "PanelAlphaSharedOverlayElement.h"

namespace MyGUI
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class _MyGUIExport PanelAlphaSharedOverlayElementFactory: public OverlayElementFactory
    {
    public:
        /** See OverlayElementFactory */
        OverlayElement* createOverlayElement(const String& instanceName)
        {
            return new PanelAlphaSharedOverlayElement(instanceName);
        }
        /** See OverlayElementFactory */
        const String& getTypeName(void) const
        {
            static String name = "PanelAlphaShared";
            return name;
        }
    };

} // namespace MyGUI

#endif // _PANELALPHASHAREDOVERLAYELEMENTFACTORY_H_
