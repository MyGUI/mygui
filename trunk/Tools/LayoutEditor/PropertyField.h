/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PROPERTY_FIELD_H__
#define __PROPERTY_FIELD_H__

#include "EditorToolTip.h"

namespace tools
{
	class PropertyField
	{
	public:
		PropertyField();

		void createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y, int _height, MyGUI::Widget* _currentWidget, EditorToolTip* _toolTip);
		void destroy();

		MyGUI::EditBox* getField();

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);
		void notifyForceApplyProperties2(MyGUI::ComboBox* _widget, size_t _index);

		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		SkinInfo getCellData(MyGUI::Widget* _sender, size_t _index);

		bool checkType(MyGUI::EditBox* _edit, const std::string& _type);
		bool isSkinExist(const std::string& _skinName);
		std::string splitString(std::string& str, char separator);
		bool checkTemplate(const std::string& _skinName);

	private:
		MyGUI::TextBox* mText;
		MyGUI::Widget* mField;
		MyGUI::Widget* mCurrentWidget;
		EditorToolTip* mToolTip;
	};

} // namespace tools

#endif // __PROPERTY_FIELD_H__
