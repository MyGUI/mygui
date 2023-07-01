/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _07f612bd_91d2_4e55_828d_a10206631a31_
#define _07f612bd_91d2_4e55_828d_a10206631a31_

#include "PropertyControl.h"
#include "TextureBrowseControl.h"
#include "Dialog.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyTexturesControl : public PropertyControl
	{
	public:
		~PropertyTexturesControl() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyChangeCoord(MyGUI::Widget* _sender);

		size_t getComboIndex(const MyGUI::UString& _name);

		void fillTextures();
		void updateTexture();

	private:
		MyGUI::ImageBox* mImage{nullptr};
		MyGUI::TextBox* mName{nullptr};
		MyGUI::ComboBox* mComboBox{nullptr};
		MyGUI::Button* mBrowse{nullptr};

		TextureBrowseControl* mTextureBrowseControl{nullptr};
		MyGUI::VectorString mTextures;
		MyGUI::IntSize mTextureSize;
	};

}

#endif
