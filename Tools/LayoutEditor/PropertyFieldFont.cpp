/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldFont.h"
#include "WidgetTypes.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "WidgetSelectorManager.h"
#include "GroupMessage.h"

namespace tools
{

	PropertyFieldFont::PropertyFieldFont(MyGUI::Widget* _parent) :
		PropertyFieldComboBox(_parent)
	{
	}

	PropertyFieldFont::~PropertyFieldFont()
	{
	}

	void PropertyFieldFont::onFillValues()
	{
		WidgetStyle::VectorString values;

		const std::string DEFAULT_STRING = "[DEFAULT]";
		values.push_back(replaceTags("ColourDefault") + DEFAULT_STRING);
		values.push_back("Default");

		MyGUI::ResourceManager::EnumeratorPtr resource = MyGUI::ResourceManager::getInstance().getEnumerator();
		while (resource.next())
		{
			MyGUI::IFont* resourceFont = resource.current().second->castType<MyGUI::IFont>(false);
			if (resourceFont != nullptr)
				values.push_back(resourceFont->getResourceName());
		}

		mField->removeAllItems();
		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

}
