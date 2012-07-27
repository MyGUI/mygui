/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _ad92adf0_6307_4e97_9d74_3ec1782971b6_
#define _ad92adf0_6307_4e97_9d74_3ec1782971b6_

#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyInt4Control :
		public PropertyControl
	{
	public:
		PropertyInt4Control();
		virtual ~PropertyInt4Control();

	protected:
		virtual void updateCaption();
		virtual void updateProperty();
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);

		bool isValidate();
		MyGUI::UString getClearValue();
		void setColour(bool _validate);

	private:
		MyGUI::TextBox* mName;
		MyGUI::EditBox* mEdit;
	};

}

#endif
