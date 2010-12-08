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
		MyGUI::EditBox* mEditSize;
		MyGUI::EditBox* mEditResolution;
		MyGUI::ComboBox* mComboAntialias;
		MyGUI::EditBox* mEditSpace;
		MyGUI::EditBox* mEditTab;
		MyGUI::EditBox* mEditCursor;
		MyGUI::EditBox* mEditDistance;
		MyGUI::EditBox* mEditOffset;
		MyGUI::EditBox* mEditRange1;
		MyGUI::EditBox* mEditRange2;
		MyGUI::EditBox* mEditHide;
		MyGUI::Button* mButtonGenerate;
		MyGUI::TextBox* mTextPix;
		MyGUI::EditBox* mEditSaveFileName;
		MyGUI::Button* mButtonSave;

		std::string mFontName;
		int mFontHeight;

		FontView* mFontView;
		TextureView* mTextureView;
	};

} // namespace demo

#endif // __FONT_PANEL_H__
