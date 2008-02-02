/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__
#define __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextEditOverlayElement.h"

namespace MyGUI
{

	class _MyGUIExport TextSimpleOverlayElement : public TextEditOverlayElement
	{

	private:
        static Ogre::String msTypeName;

	public:
        /** Constructor. */
		TextSimpleOverlayElement(const Ogre::String& name);

        /// Inherited function
		virtual void initialise(void);
        /// Inherited function
		virtual void updatePositionGeometry();

		/** See OverlayElement. */
        virtual const Ogre::String& getTypeName(void) const;
		
	}; // class TextSimpleOverlayElement : public TextEditOverlayElement

} // namespace MyGUI

#endif // __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__
