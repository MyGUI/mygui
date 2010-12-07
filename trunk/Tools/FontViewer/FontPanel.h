/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __FONT_PANEL_H__
#define __FONT_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "FontView.h"
#include "TextureView.h"

namespace demo
{

	class FontPanel :
		public wraps::BaseLayout
	{
	public:
		FontPanel();
		virtual ~FontPanel();

	private:
		void update();
		void notifyMouseButtonClick(MyGUI::Widget* _widget);
		void generateFontTTFXml(MyGUI::xml::ElementPtr _root, const std::string& _fontName);
		void generateFontManualXml(MyGUI::xml::ElementPtr _root, const std::string& _textureName, const std::string& _fontName);

		void saveTexture(const std::string& _fontName, const std::string& _fileName);
		void saveFontTTFXml(const std::string& _fontName, const std::string& _fileName);
		void saveFontManualXml(const std::string& _fontName, const std::string& _textureName, const std::string& _fileName);

		void addCode(MyGUI::xml::Element* _node, MyGUI::Char _code, MyGUI::IFont* _font);
		MyGUI::IntCoord getCoord(const MyGUI::FloatRect& _rect, int _textureWidth, int _textureHeight, int _charWidth, int _fontHeight);

	private:
		MyGUI::ComboBox* mComboFont;
		MyGUI::Edit* mEditSize;
		MyGUI::Edit* mEditResolution;
		MyGUI::ComboBox* mComboAntialias;
		MyGUI::Edit* mEditSpace;
		MyGUI::Edit* mEditTab;
		MyGUI::Edit* mEditCursor;
		MyGUI::Edit* mEditDistance;
		MyGUI::Edit* mEditOffset;
		MyGUI::Edit* mEditRange1;
		MyGUI::Edit* mEditRange2;
		MyGUI::Edit* mEditHide;
		MyGUI::Button* mButtonGenerate;
		MyGUI::TextBox* mTextPix;
		MyGUI::Edit* mEditSaveFileName;
		MyGUI::Button* mButtonSave;

		std::string mFontName;
		int mFontHeight;

		FontView* mFontView;
		TextureView* mTextureView;
	};

} // namespace demo

#endif // __FONT_PANEL_H__
