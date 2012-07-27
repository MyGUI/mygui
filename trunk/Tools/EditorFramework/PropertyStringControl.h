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

	class MYGUI_EXPORT_DLL PropertyStringControl :
		public PropertyControl
	{
	public:
		PropertyStringControl();
		virtual ~PropertyStringControl();

	protected:
		virtual void updateCaption();
		virtual void updateProperty();
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);

		MyGUI::UString getClearValue();

	private:
		MyGUI::TextBox* mName;
		MyGUI::EditBox* mEdit;
	};

}

#endif
