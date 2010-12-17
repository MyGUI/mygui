/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldType.h"
#include "WidgetTypes.h"
#include "EditorWidgets.h"

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

	/*void PropertyFieldType::onAction(const std::string& _value)
	{
		eventAction(mType, _value);
	}*/

} // namespace tools
