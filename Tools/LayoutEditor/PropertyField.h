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
	class IPropertyField
	{
	public:
		virtual ~IPropertyField() = 0 { }

		virtual void initialise(MyGUI::Widget* _parent, const std::string& _type, MyGUI::Widget* _targetWidget) = 0;

		virtual void setValue(const std::string& _value) = 0;
		virtual void setName(const std::string& _value) = 0;

		virtual MyGUI::IntSize getContentSize() = 0;
		virtual void setCoord(const MyGUI::IntCoord& _coord) = 0;
	};
	
	class PropertyField :
		public IPropertyField
	{
	public:
		PropertyField();
		virtual ~PropertyField();

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
		std::string mType;
	};

} // namespace tools

#endif // __PROPERTY_FIELD_H__
