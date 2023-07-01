/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _ecee333e_5aa3_409f_9c32_10117a8bcbfc_
#define _ecee333e_5aa3_409f_9c32_10117a8bcbfc_

#include <MyGUI.h>

namespace tools
{

	using PropertyFieldActionDelegate = MyGUI::delegates::Delegate<std::string_view, std::string_view, bool>;

	class IPropertyField
	{
	public:
		virtual ~IPropertyField() = default;

		virtual void initialise(std::string_view _type) = 0;

		virtual void setTarget(MyGUI::Widget* _targetWidget) = 0;
		virtual void setValue(std::string_view _value) = 0;
		virtual void setName(std::string_view _value) = 0;

		virtual void setVisible(bool _value) = 0;
		virtual bool getVisible() = 0;

		virtual MyGUI::IntSize getContentSize() = 0;
		virtual void setCoord(const MyGUI::IntCoord& _coord) = 0;

		PropertyFieldActionDelegate eventAction;
	};

}

#endif
