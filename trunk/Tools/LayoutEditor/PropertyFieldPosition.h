/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _267e3805_b900_4be2_a0bd_6f6ea3d0262d_
#define _267e3805_b900_4be2_a0bd_6f6ea3d0262d_

#include "EditorToolTip.h"
#include "BaseLayout/BaseLayout.h"
#include "IPropertyField.h"
#include "sigslot.h"

namespace tools
{

	class PropertyFieldPosition :
		public wraps::BaseLayout,
		public IPropertyField,
		public sigslot::has_slots<>
	{
	public:
		PropertyFieldPosition(MyGUI::Widget* _parent);
		virtual ~PropertyFieldPosition();

		virtual void initialise(const std::string& _type);

		virtual void setTarget(MyGUI::Widget* _currentWidget);
		virtual void setValue(const std::string& _value);
		virtual void setName(const std::string& _value);

		virtual void setVisible(bool _value);
		virtual bool getVisible();

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyToggleRelativeMode();

		void commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result);
		void notifyPropertyChangeCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coordValue, const std::string& _owner);
		void updatePositionCaption();
		void updateButton();

	protected:
		virtual bool onCheckValue();
		virtual void onAction(const std::string& _value, bool _force);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
		MyGUI::Button* mButton;
		MyGUI::Widget* mCurrentWidget;
		std::string mType;
		std::string mName;
	};

}

#endif
