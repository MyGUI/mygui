/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_MANUAL_FONT_H_
#define MYGUI_RESOURCE_MANUAL_FONT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IFont.h"

#include <unordered_map>

namespace MyGUI
{

	class MYGUI_EXPORT ResourceManualFont :
		public IFont
	{
		MYGUI_RTTI_DERIVED( ResourceManualFont )

	public:
		ResourceManualFont();

		void deserialization(xml::ElementPtr _node, Version _version) override;

		// Returns the glyph info for the specified code point, or the glyph info for a substitute glyph if the code point does not
		// exist in this font. Returns nullptr if the code point does not exist and there is no substitute glyph available.
		GlyphInfo* getGlyphInfo(Char _id) override;

		ITexture* getTextureFont() override;

		// дефолтная высота, указанная в настройках шрифта
		int getDefaultHeight() override;

		// Manual loading methods, not needed when loading from XML
		// Set the source texture by name
		void setSource(const std::string& value);
		// Set the source texture directly
		// Note: the user is responsible for deallocation of the texture.
		void setTexture(MyGUI::ITexture* texture);
		// Set the default height of the font
		void setDefaultHeight(int value);
		// Add a glyph for character 'id'
		void addGlyphInfo(Char id, const GlyphInfo& info);

	private:
		// Loads the texture specified by mSource.
		void loadTexture();

		// A map of code points to glyph info objects.
		typedef std::unordered_map<Char, GlyphInfo> CharMap;

		// The following variables are set directly from values specified by the user.
		std::string mSource; // Source (filename) of the font.

		// The following variables are calculated automatically.
		int mDefaultHeight; // The nominal height of the font in pixels.
		GlyphInfo* mSubstituteGlyphInfo; // The glyph info to use as a substitute for code points that don't exist in the font.
		MyGUI::ITexture* mTexture; // The texture that contains all of the rendered glyphs in the font.

		CharMap mCharMap; // A map of code points to glyph info objects.
	};

} // namespace MyGUI

#endif // MYGUI_RESOURCE_MANUAL_FONT_H_
