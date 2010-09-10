/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __TEXTURE_BROWSE_CONTROL_H__
#define __TEXTURE_BROWSE_CONTROL_H__

#include <MyGUI.h>
#include "Dialog.h"

namespace tools
{

	ATTRIBUTE_CLASS_LAYOUT(TextureBrowseControl, "TextureBrowseControl.layout");
	class TextureBrowseControl :
		public Dialog
	{
	public:
		TextureBrowseControl();
		virtual ~TextureBrowseControl();

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyMouseButtonClickOk(MyGUI::Widget* _sender);
		void notifyMouseButtonClickCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(TextureBrowseControl, mOk, "OK");
		MyGUI::Button* mOk;

		ATTRIBUTE_FIELD_WIDGET_NAME(TextureBrowseControl, mCancel, "Cancel");
		MyGUI::Button* mCancel;
	};

} // namespace tools

#endif // __TEXTURE_BROWSE_CONTROL_H__
