/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldName.h"
#include "Localise.h"
#include "EditorWidgets.h"

namespace tools
{

	PropertyFieldName::PropertyFieldName(MyGUI::Widget* _parent) :
		PropertyFieldEditBox(_parent)
	{
	}

	PropertyFieldName::~PropertyFieldName()
	{
	}

	void PropertyFieldName::onAction(const std::string& _value, bool _force)
	{
		const std::string DEFAULT_STRING = "[DEFAULT]";
		std::string DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;

		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);

		bool goodData = onCheckValue();

		widgetContainer->name = _value;
		ew->invalidateWidgets();
	}

} // namespace tools
