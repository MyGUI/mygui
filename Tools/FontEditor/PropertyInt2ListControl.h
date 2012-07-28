/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _903f2077_c065_4db6_92a4_7c72ee220b5c_
#define _903f2077_c065_4db6_92a4_7c72ee220b5c_

#include "PropertyControl.h"
#include "TextFieldControl.h"

namespace tools
{

	class PropertyInt2ListControl :
		public PropertyControl
	{
	public:
		PropertyInt2ListControl();
		virtual ~PropertyInt2ListControl();

	protected:
		virtual void updateCaption();
		virtual void updateProperty();
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		std::string getValue();
		void setValue(const std::string& _value);

		void notifyClickAdd(MyGUI::Widget* _sender);
		void notifyClickDelete(MyGUI::Widget* _sender);

		void notifyEndDialog(Dialog* _sender, bool _result);

	private:
		MyGUI::TextBox* mName;
		MyGUI::ListBox* mList;
		MyGUI::Button* mAdd;
		MyGUI::Button* mDelete;

		TextFieldControl* mTextFieldControl;
	};

}

#endif
