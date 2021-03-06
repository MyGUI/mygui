/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _c1d21eed_2872_4685_9244_e8e11dff7029_
#define _c1d21eed_2872_4685_9244_e8e11dff7029_

#include "EditorToolTip.h"
#include "BaseLayout/BaseLayout.h"
#include "IPropertyField.h"

namespace tools
{

	class PropertyFieldEditBox :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldEditBox(MyGUI::Widget* _parent);

		void initialise(const std::string& _type) override;

		void setTarget(MyGUI::Widget* _currentWidget) override;
		void setValue(const std::string& _value) override;
		void setName(const std::string& _value) override;

		void setVisible(bool _value) override;
		bool getVisible() override;

		MyGUI::IntSize getContentSize() override;
		void setCoord(const MyGUI::IntCoord& _coord) override;

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);

	protected:
		virtual bool onCheckValue();
		virtual void onAction(const std::string& _value, bool _final);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
		MyGUI::Widget* mCurrentWidget;
		std::string mType;
		std::string mName;
	};

}

#endif
