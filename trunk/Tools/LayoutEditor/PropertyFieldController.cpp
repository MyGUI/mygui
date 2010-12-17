/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldController.h"
#include "Localise.h"
#include "EditorWidgets.h"

namespace tools
{

	PropertyFieldController::PropertyFieldController(MyGUI::Widget* _parent) :
		PropertyFieldEditBox(_parent)
	{
	}

	PropertyFieldController::~PropertyFieldController()
	{
	}

	void PropertyFieldController::onAction(const std::string& _value, bool _force)
	{
		const std::string DEFAULT_STRING = "[DEFAULT]";
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		bool goodData = onCheckValue();

		std::string tmp = mName;

		int n = MyGUI::utility::parseValue<int>(splitString(tmp, ' '));
		std::string key = splitString(tmp, ' ');
		widgetContainer->mController[n]->mProperty[key] = _value;
	}

	std::string PropertyFieldController::splitString(std::string& str, char separator)
	{
		size_t spaceIdx = str.find(separator);
		if (spaceIdx == std::string::npos)
		{
			std::string tmp = str;
			str.clear();
			return tmp;
		}
		else
		{
			std::string tmp = str.substr(0, spaceIdx);
			str.erase(0, spaceIdx + 1);
			return tmp;
		}
	}

} // namespace tools
