/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXT_FIELD_CONTROL_H__
#define __TEXT_FIELD_CONTROL_H__

#include "BaseManager.h"
#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class TextFieldControl :
		public wraps::BaseLayout
	{
	public:
		TextFieldControl();
		virtual ~TextFieldControl();

		void setCaption(const MyGUI::UString& _value);

		void setTextField(const MyGUI::UString& _value);
		const MyGUI::UString& getTextField();

	private:
		void notifyOk(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);

	private:
		MyGUI::StaticText* mText;
		MyGUI::Button* mOk;
		MyGUI::Button* mCancel;
	};

} // namespace tools

#endif // __TEXT_FIELD_CONTROL_H__
