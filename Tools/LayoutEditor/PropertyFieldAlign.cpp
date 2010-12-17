/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldAlign.h"
#include "EditorWidgets.h"
#include "UndoManager.h"

namespace tools
{

	PropertyFieldAlign::PropertyFieldAlign()
	{
	}

	PropertyFieldAlign::~PropertyFieldAlign()
	{
	}

	void PropertyFieldAlign::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		widgetContainer->align = _value;
		widgetContainer->widget->setAlign(MyGUI::Align::parse(_value));

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

} // namespace tools
