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

	class FontPanel : public wraps::BaseLayout
	{
	public:
		FontPanel();
		virtual ~FontPanel();

	private:
		void update();
		void notifyMouseButtonClick(MyGUI::Widget* _widget);
		void generateFontTTFXml(MyGUI::xml::ElementPtr _root);

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
		MyGUI::StaticText* mTextPix;
		MyGUI::Edit* mEditSaveFileName;
		MyGUI::Button* mButtonSave;

		std::string mFontName;
		int mFontHeight;

		FontView* mFontView;
		TextureView* mTextureView;

	};

} // namespace demo

#endif // __FONT_PANEL_H__
