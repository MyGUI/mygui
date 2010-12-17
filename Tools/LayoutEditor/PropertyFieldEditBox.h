/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_EDIT_BOX_H__
#define __PROPERTY_FIELD_EDIT_BOX_H__

#include "EditorToolTip.h"
#include "IPropertyField.h"

namespace tools
{
	class PropertyFieldEditBox :
		public IPropertyField
	{
	public:
		PropertyFieldEditBox();
		virtual ~PropertyFieldEditBox();

		virtual void initialise(MyGUI::Widget* _window, const std::string& _type, MyGUI::Widget* _currentWidget);

		virtual void setValue(const std::string& _value);
		virtual void setName(const std::string& _value);

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void destroy();

		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);

		bool checkType(MyGUI::EditBox* _edit, const std::string& _type);
		std::string splitString(std::string& str, char separator);

	private:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
		MyGUI::Widget* mCurrentWidget;
		std::string mType;
	};

} // namespace tools

#endif // __PROPERTY_FIELD_EDIT_BOX_H__
