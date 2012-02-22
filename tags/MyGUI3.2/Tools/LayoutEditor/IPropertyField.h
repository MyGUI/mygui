/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __I_PROPERTY_FIELD_H__
#define __I_PROPERTY_FIELD_H__

#include <MyGUI.h>

namespace tools
{
	typedef MyGUI::delegates::CDelegate3<const std::string&, const std::string&, bool> PropertyFieldActionDelegate;

	class IPropertyField
	{
	public:
		virtual ~IPropertyField() { }

		virtual void initialise(const std::string& _type) = 0;

		virtual void setTarget(MyGUI::Widget* _targetWidget) = 0;
		virtual void setValue(const std::string& _value) = 0;
		virtual void setName(const std::string& _value) = 0;

		virtual void setVisible(bool _value) = 0;
		virtual bool getVisible() = 0;

		virtual MyGUI::IntSize getContentSize() = 0;
		virtual void setCoord(const MyGUI::IntCoord& _coord) = 0;

		PropertyFieldActionDelegate eventAction;
	};
	
} // namespace tools

#endif // __I_PROPERTY_FIELD_H__
