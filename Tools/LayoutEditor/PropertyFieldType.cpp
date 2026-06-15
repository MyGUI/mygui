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

	void PropertyFieldType::onFillValues()
	{
		WidgetStyle::VectorString values;

		VectorWidgetType types = WidgetTypes::getInstance().getWidgetTypes();
		for (auto& type : types)
		{
			bool exist = MyGUI::WidgetManager::getInstance().isFactoryExist(type->name);
			if (exist && !type->internalType)
				values.push_back(type->name);
		}

		mField->removeAllItems();
		for (auto& value : values)
			mField->addItem(value);
		mField->beginToItemFirst();
	}

}
