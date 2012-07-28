/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _903f2077_c065_4db6_92a4_7c72ee220b5c_
#define _903f2077_c065_4db6_92a4_7c72ee220b5c_

#include "PropertyControl.h"

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
		//void notifyEditTextChange(MyGUI::EditBox* _sender);

		//bool isValidate();
		//MyGUI::UString getClearValue();
		//void setColour(bool _validate);

		std::string getValue();
		void setValue(const std::string& _value);

	private:
		MyGUI::TextBox* mName;
		MyGUI::ListBox* mList;
	};

}

#endif
