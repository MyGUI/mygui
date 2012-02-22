/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __FONT_PANEL_H__
#define __FONT_PANEL_H__

#include <MyGUI.h>
#include "MyGUI_ResourceTrueTypeFont.h"
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
		void initializeEditBox(MyGUI::EditBox* _editBox);
		void initializeComboBox(MyGUI::ComboBox* _comboBox);

		void update();
		void enableSave(bool _enable, const MyGUI::UString& fileName = MyGUI::UString());

		void notifyTextChange(MyGUI::EditBox* _sender);
		void notifyComboBoxChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyComboBoxKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);
		void notifyComboBoxKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);
		void notifyMouseButtonClick(MyGUI::Widget* _widget);

		void generateFontTTFXml(MyGUI::xml::ElementPtr _root, const std::string& _fontName);
		void generateFontManualXml(MyGUI::xml::ElementPtr _root, const std::string& _textureName, const std::string& _fontName);

		void saveTexture(const std::string& _fontName, const std::string& _fileName);
		void saveFontTTFXml(const std::string& _fontName, const std::string& _fileName);
		void saveFontManualXml(const std::string& _fontName, const std::string& _textureName, const std::string& _fileName);

		void removeFont(const std::string& _fontName);

		void addCode(MyGUI::xml::Element* _node, MyGUI::Char _code, MyGUI::ResourceTrueTypeFont* _font, bool _isSubstitute);

	private:
		MyGUI::ComboBox* mComboFont;
		MyGUI::EditBox* mEditSize;
		MyGUI::ComboBox* mComboResolution;
		MyGUI::ComboBox* mComboAntialias;
		MyGUI::ComboBox* mComboSpace;
		MyGUI::ComboBox* mComboTab;
		MyGUI::ComboBox* mComboOffset;
		MyGUI::EditBox* mEditRange1A;
		MyGUI::EditBox* mEditRange1B;
		MyGUI::EditBox* mEditRange2A;
		MyGUI::EditBox* mEditRange2B;
		MyGUI::EditBox* mEditHideA;
		MyGUI::EditBox* mEditHideB;
		MyGUI::ComboBox* mComboSubstituteCode;
		MyGUI::Button* mButtonGenerate;
		MyGUI::EditBox* mEditSaveFileName;
		MyGUI::Button* mButtonSave;

		std::string mFontName;
		int mFontHeight;

		FontView* mFontView;
		TextureView* mTextureView;
	};

} // namespace demo

#endif // __FONT_PANEL_H__
