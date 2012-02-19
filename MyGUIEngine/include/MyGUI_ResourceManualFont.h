/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_RESOURCE_MANUAL_FONT_H__
#define __MYGUI_RESOURCE_MANUAL_FONT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IFont.h"

namespace MyGUI
{

	class MYGUI_EXPORT ResourceManualFont :
		public IFont
	{
		MYGUI_RTTI_DERIVED( ResourceManualFont )

	public:
		ResourceManualFont();
		virtual ~ResourceManualFont();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		// Returns the glyph info for the specified code point, or the glyph info for a substitute glyph if the code point does not
		// exist in this font. Returns nullptr if the code point does not exist and there is no substitute glyph available.
		virtual GlyphInfo* getGlyphInfo(Char _id);

		virtual ITexture* getTextureFont();

		// дефолтная высота, указанная в настройках шрифта
		virtual int getDefaultHeight();

	private:
		// Loads the texture specified by mSource.
		void loadTexture();

		// A map of code points to glyph info objects.
		typedef std::map<Char, GlyphInfo> CharMap;

		// The following variables are set directly from values specified by the user.
		std::string mSource; // Source (filename) of the font.

		// The following variables are calculated automatically.
		int mDefaultHeight; // The nominal height of the font in pixels.
		GlyphInfo* mSubstituteGlyphInfo; // The glyph info to use as a substitute for code points that don't exist in the font.
		MyGUI::ITexture* mTexture; // The texture that contains all of the rendered glyphs in the font.

		CharMap mCharMap; // A map of code points to glyph info objects.
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_MANUAL_FONT_H__
