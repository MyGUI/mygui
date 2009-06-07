/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
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
		void notifyMouseButtonClick(MyGUI::WidgetPtr _widget);

	private:
		MyGUI::ComboBoxPtr mComboFont;
		MyGUI::EditPtr mEditSize;
		MyGUI::EditPtr mEditResolution;
		MyGUI::ComboBoxPtr mComboAntialias;
		MyGUI::EditPtr mEditSpace;
		MyGUI::EditPtr mEditTab;
		MyGUI::EditPtr mEditCursor;
		MyGUI::EditPtr mEditDistance;
		MyGUI::EditPtr mEditOffset;
		MyGUI::ButtonPtr mButtonGenerate;
		MyGUI::StaticTextPtr mTextPix;
		MyGUI::EditPtr mEditSaveFileName;
		MyGUI::ButtonPtr mButtonSave;

		std::string mFontName;
		int mFontHeight;

		FontView * mFontView;
		TextureView * mTextureView;

	};

} // namespace demo

#endif // __FONT_PANEL_H__

