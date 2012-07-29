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

	class PropertyFieldComboBox :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldComboBox(MyGUI::Widget* _parent);
		virtual ~PropertyFieldComboBox();

		virtual void initialise(const std::string& _type);

		virtual void setTarget(MyGUI::Widget* _currentWidget);
		virtual void setValue(const std::string& _value);
		virtual void setName(const std::string& _value);

		virtual void setVisible(bool _value);
		virtual bool getVisible();

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	protected:
		virtual void onFillValues();
		virtual void onAction(const std::string& _value, bool _final);

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender);
		void notifyForceApplyProperties2(MyGUI::ComboBox* _widget, size_t _index);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::ComboBox* mField;
		MyGUI::Widget* mCurrentWidget;
		std::string mType;
		std::string mName;
	};

}

#endif
