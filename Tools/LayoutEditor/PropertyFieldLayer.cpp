/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldLayer.h"
#include "EditorWidgets.h"
#include "UndoManager.h"

namespace tools
{

	PropertyFieldLayer::PropertyFieldLayer(MyGUI::Widget* _parent) :
		PropertyFieldComboBox(_parent)
	{
	}

	PropertyFieldLayer::~PropertyFieldLayer()
	{
	}

	void PropertyFieldLayer::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		widgetContainer->setLayerName(_value);

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

} // namespace tools
