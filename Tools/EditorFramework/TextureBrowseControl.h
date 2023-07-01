/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/

#ifndef _2122a0ba_a0a9_497c_8186_e1fa60400305_
#define _2122a0ba_a0a9_497c_8186_e1fa60400305_

#include <MyGUI.h>
#include "Dialog.h"
#include "ItemBox/BaseItemBox.h"
#include "TextureBrowseCell.h"
#include "Control.h"

namespace tools
{

	ATTRIBUTE_CLASS_LAYOUT(TextureBrowseControl, "TextureBrowseControl.layout");
	class MYGUI_EXPORT_DLL TextureBrowseControl : public Dialog, public Control
	{
	public:
		~TextureBrowseControl() override;

		const std::string& getTextureName() const;
		void setTextureName(std::string_view _value);

		void setTextures(const MyGUI::VectorString& _textures);

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

		void onDoModal() override;
		void onEndModal() override;

	private:
		void notifyMouseButtonClickOk(MyGUI::Widget* _sender);
		void notifyMouseButtonClickCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name);
		void notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index);
		void notifySelectItemAccept(MyGUI::ItemBox* _sender, size_t _index);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(TextureBrowseControl, mOk, "OK");
		MyGUI::Button* mOk{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(TextureBrowseControl, mCancel, "Cancel");
		MyGUI::Button* mCancel{nullptr};

		wraps::BaseItemBox<TextureBrowseCell>* mTextures{nullptr};

		std::string mCurrentTextureName;
	};

}

#endif
