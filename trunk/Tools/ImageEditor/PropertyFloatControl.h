/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _ac3769fd_b132_461a_b607_737e76f02a6e_
#define _ac3769fd_b132_461a_b607_737e76f02a6e_

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyFloatControl :
		public PropertyControl
	{
	public:
		PropertyFloatControl();
		virtual ~PropertyFloatControl();

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
