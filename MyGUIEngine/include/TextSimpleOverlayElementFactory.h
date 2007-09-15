#ifndef _TEXTSIMPLEOVERLAYELEMENTFACTORY_H_
#define _TEXTSIMPLEOVERLAYELEMENTFACTORY_H_

#include "Prerequest.h"
#include <OgreOverlayElementFactory.h>
#include "TextSimpleOverlayElement.h"

namespace widget
{

	/** Factory for creating TextSimpleOverlayElementFactory instances. */
    class _MyGUIExport TextSimpleOverlayElementFactory: public OverlayElementFactory
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

#endif
