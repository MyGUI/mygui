/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _1ff1ce31_9c20_47d9_ab7e_79bf431cc819_
#define _1ff1ce31_9c20_47d9_ab7e_79bf431cc819_

#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyBoolControl :
		public PropertyControl
	{
	public:
		PropertyBoolControl();
		virtual ~PropertyBoolControl();

	protected:
		virtual void updateCaption();
		virtual void updateProperty();
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::TextBox* mName;
		MyGUI::ComboBox* mComboBox;
	};

}

#endif
