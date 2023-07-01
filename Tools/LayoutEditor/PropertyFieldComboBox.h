/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _eca0d7d5_24a7_4e9f_8715_a6f2788d91aa_
#define _eca0d7d5_24a7_4e9f_8715_a6f2788d91aa_

#include "EditorToolTip.h"
#include "BaseLayout/BaseLayout.h"
#include "IPropertyField.h"

namespace tools
{

	class PropertyFieldComboBox : public wraps::BaseLayout, public IPropertyField
	{
	public:
		PropertyFieldComboBox(MyGUI::Widget* _parent);

		void initialise(std::string_view _type) override;

		void setTarget(MyGUI::Widget* _currentWidget) override;
		void setValue(std::string_view _value) override;
		void setName(std::string_view _value) override;

		void setVisible(bool _value) override;
		bool getVisible() override;

		MyGUI::IntSize getContentSize() override;
		void setCoord(const MyGUI::IntCoord& _coord) override;

	protected:
		virtual void onFillValues();
		virtual void onAction(std::string_view _value, bool _final);

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender);
		void notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index);

	protected:
		MyGUI::TextBox* mText{nullptr};
		MyGUI::ComboBox* mField{nullptr};
		MyGUI::Widget* mCurrentWidget{nullptr};
		std::string mType;
		std::string mName;
	};

}

#endif
