/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _3b820011_7157_4d96_87d3_07c4e199d2c8_
#define _3b820011_7157_4d96_87d3_07c4e199d2c8_

#include "PropertyControl.h"

namespace tools
{

	class PropertyAlignControl :
		public PropertyControl
	{
	public:
		PropertyAlignControl();
		~PropertyAlignControl() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName) override;

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

	private:
		MyGUI::TextBox* mName;
		MyGUI::ComboBox* mComboBox;
	};

}

#endif
