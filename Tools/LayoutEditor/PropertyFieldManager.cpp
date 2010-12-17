/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "PropertyFieldManager.h"
#include "PropertyFieldComboBox.h"
#include "PropertyFieldEditBox.h"
#include "PropertyFieldEditBoxAccept.h"

template <> tools::PropertyFieldManager* MyGUI::Singleton<tools::PropertyFieldManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::PropertyFieldManager>::mClassTypeName("PropertyFieldManager");

namespace tools
{

	PropertyFieldManager::PropertyFieldManager()
	{
	}

	PropertyFieldManager::~PropertyFieldManager()
	{
	}

	void PropertyFieldManager::initialise()
	{
	}

	void PropertyFieldManager::shutdown()
	{
	}

	IPropertyField* PropertyFieldManager::createPropertyField(MyGUI::Widget* _window, const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		enum PropertyType
		{
			PropertyType_Edit,
			PropertyType_ComboBox,
			PropertyType_EditAcceptOnly,
			PropertyType_Count
		};

		PropertyType widget_for_type = PropertyType_Count;

		if ("Name" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Type" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("Skin" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("Position" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Layer" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("String" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("StringAccept" == _type)
			widget_for_type = PropertyType_EditAcceptOnly;
		else if ("Align" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("FileName" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("1 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("2 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("4 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("alpha" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("1 float" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("2 float" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Colour" == _type)
			widget_for_type = PropertyType_Edit;
		else
			widget_for_type = PropertyType_ComboBox;

		IPropertyField* result = nullptr;
		if (widget_for_type == PropertyType_Edit)
			result = new PropertyFieldEditBox();
		else if (widget_for_type == PropertyType_EditAcceptOnly)
			result = new PropertyFieldEditBoxAccept();
		else
			result = new PropertyFieldComboBox();

		result->initialise(_window,  _type, _currentWidget);
		return result;
	}

} // namespace tools
