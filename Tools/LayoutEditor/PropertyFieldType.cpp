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
		{
			bool exist = MyGUI::WidgetManager::getInstance().isFactoryExist((*iter)->name);
			if (exist && !(*iter)->internalType)
				values.push_back((*iter)->name);
		}

		mField->removeAllItems();
		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

}
