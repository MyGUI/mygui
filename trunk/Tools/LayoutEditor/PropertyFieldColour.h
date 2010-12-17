/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_COLOUR_H__
#define __PROPERTY_FIELD_COLOUR_H__

#include "EditorToolTip.h"
#include "BaseLayout/BaseLayout.h"
#include "IPropertyField.h"

namespace tools
{
	class PropertyFieldColour :
		public wraps::BaseLayout,
		public IPropertyField
	{
	public:
		PropertyFieldColour(MyGUI::Widget* _parent);
		virtual ~PropertyFieldColour();

		virtual void initialise(const std::string& _type, MyGUI::Widget* _currentWidget);

		virtual void setValue(const std::string& _value);
		virtual void setName(const std::string& _value);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);

		void updateColourPlace(bool _success);

	protected:
		virtual bool onCheckValue();
		virtual void onAction(const std::string& _value);

	protected:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
		MyGUI::Widget* mColourPlace;
		MyGUI::Widget* mCurrentWidget;
		std::string mType;
		std::string mName;
	};

} // namespace tools

#endif // __PROPERTY_FIELD_COLOUR_H__
