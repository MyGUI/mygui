/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldType.h"
#include "WidgetTypes.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "WidgetSelectorManager.h"
#include "GroupMessage.h"

namespace tools
{

	PropertyFieldType::PropertyFieldType(MyGUI::Widget* _parent) :
		PropertyFieldComboBox(_parent)
	{
	}

	PropertyFieldType::~PropertyFieldType()
	{
	}

	void PropertyFieldType::onFillValues()
	{
		WidgetStyle::VectorString values;

		VectorWidgetType types = WidgetTypes::getInstance().getWidgetTypes();
		for (VectorWidgetType::iterator iter = types.begin(); iter != types.end(); ++iter)
			values.push_back((*iter)->name);

		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldType::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		widgetContainer->type = _value;

		WidgetSelectorManager::getInstance().saveSelectedWidget();

		MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(savedDoc);
		delete savedDoc;

		WidgetSelectorManager::getInstance().restoreSelectedWidget();

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

} // namespace tools
