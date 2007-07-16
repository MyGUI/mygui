#pragma once

#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>


namespace widget
{

	#define POS_TEX_BINDING 0
    #define COLOUR_BINDING 1
	#define UNICODE_NEL 0x0085
	#define UNICODE_CR 0x000D
	#define UNICODE_LF 0x000A
	#define UNICODE_SPACE 0x0020
	#define UNICODE_ZERO 0x0030

	using namespace Ogre;

	class TextSimpleOverlayElement : public TextAreaOverlayElement
	{

	public:
		TextSimpleOverlayElement(const String& name) : TextAreaOverlayElement(name) {}

		void getTextSize(int & _cx, int & _cy)
		{

			if (mpFont.isNull())
			{
				// not initialised yet, probably due to the order of creation in a template
				return;
			}

			// Derive space with from a number 0
			if (mSpaceWidth == 0)
			{
				mSpaceWidth = mpFont->getGlyphAspectRatio(UNICODE_ZERO) * mCharHeight * 2.0 * mViewportAspectCoef;
			}

			float height = 12;
			float cy = height;
			float cx = 0;
			float len = 0;

			DisplayString::const_iterator iend = mCaption.end();

			for (DisplayString::const_iterator index = mCaption.begin(); index != iend; index++) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if ( character == UNICODE_SPACE) len += mSpaceWidth;
				else if (character == 10) { // перевод строки

					if (len > cx) cx = len;
					len = 0;
					cy += height;

				} else len += mpFont->getGlyphAspectRatio(character) * height;
			}

			if (len > cx) cx = len;
	

			_cx = (int)cx;
			_cy = (int)cy;

		}

	};

} // namespace widget