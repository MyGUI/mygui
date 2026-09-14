#ifndef MYGUI_UNITTEST_FIXED_FONT_H_
#define MYGUI_UNITTEST_FIXED_FONT_H_

#include "MyGUI_IFont.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_FontManager.h"

namespace unittest
{

	// Deliberately simple metrics: every character advances 10 pixels on a 20-pixel line.
	class FixedFont : public MyGUI::IFont
	{
	public:
		const MyGUI::GlyphInfo* getGlyphInfo(MyGUI::Char) const override
		{
			return &mGlyph;
		}

		MyGUI::ITexture* getTextureFont() const override
		{
			return nullptr;
		}

		int getDefaultHeight() const override
		{
			return 20;
		}

	private:
		MyGUI::GlyphInfo mGlyph{0, 10, 20, 10};
	};

	inline void registerFixedFont()
	{
		auto* font = new FixedFont();
		font->setResourceName("UnitTestFixedFont");
		MyGUI::ResourceManager::getInstance().addResource(font);
		MyGUI::FontManager::getInstance().setDefaultFont(font->getResourceName());
	}

}

#endif // MYGUI_UNITTEST_FIXED_FONT_H_
