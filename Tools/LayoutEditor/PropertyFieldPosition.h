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

	class PropertyFieldPosition : public wraps::BaseLayout, public IPropertyField, public sigslot::has_slots<>
	{
	public:
		PropertyFieldPosition(MyGUI::Widget* _parent);
		~PropertyFieldPosition() override;

		void initialise(std::string_view _type) override;

		void setTarget(MyGUI::Widget* _currentWidget) override;
		void setValue(std::string_view _value) override;
		void setName(std::string_view _value) override;

		void setVisible(bool _value) override;
		bool getVisible() override;

		MyGUI::IntSize getContentSize() override;
		void setCoord(const MyGUI::IntCoord& _coord) override;

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyToggleRelativeMode();

		void commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result);
		void notifyPropertyChangeCoord(
			MyGUI::Widget* _widget,
			const MyGUI::IntCoord& _coordValue,
			std::string_view _owner);
		void updatePositionCaption();
		void updateButton();

	protected:
		virtual bool onCheckValue();
		virtual void onAction(std::string_view _value, bool _force);

	protected:
		MyGUI::TextBox* mText{nullptr};
		MyGUI::EditBox* mField{nullptr};
		MyGUI::Button* mButton{nullptr};
		MyGUI::Widget* mCurrentWidget{nullptr};
		std::string mType;
		std::string mName;
	};

}

#endif
