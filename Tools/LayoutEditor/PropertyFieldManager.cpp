/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "PropertyFieldManager.h"
#include "PropertyFieldComboBox.h"
#include "PropertyFieldEditBox.h"
#include "PropertyFieldSkin.h"
#include "PropertyFieldType.h"
#include "PropertyFieldFont.h"
#include "PropertyFieldAlign.h"
#include "PropertyFieldLayer.h"
#include "PropertyFieldName.h"
#include "PropertyFieldFileName.h"
#include "PropertyFieldNumeric.h"
#include "PropertyFieldAlpha.h"
#include "PropertyFieldPosition.h"
//#include "PropertyFieldController.h"

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
		IPropertyField* result = nullptr;

		/*if (MyGUI::utility::startWith(_type, "Controller"))
			result = new PropertyFieldController(_window);
		else */if ("Name" == _type)
			result = new PropertyFieldEditBox(_window);
		else if ("Type" == _type)
			result = new PropertyFieldType(_window);
		else if ("Skin" == _type)
			result = new PropertyFieldSkin(_window);
		else if ("Font" == _type)
			result = new PropertyFieldFont(_window);
		else if ("Position" == _type)
			result = new PropertyFieldPosition(_window);
		else if ("Layer" == _type)
			result = new PropertyFieldComboBox(_window);
		else if ("String" == _type)
			result = new PropertyFieldEditBox(_window);
		else if ("Align" == _type)
			result = new PropertyFieldComboBox(_window);
		else if ("FileName" == _type)
			result = new PropertyFieldFileName(_window);
		else if ("1 int" == _type)
			result = new PropertyFieldNumeric(_window);
		else if ("2 int" == _type)
			result = new PropertyFieldNumeric(_window);
		else if ("4 int" == _type)
			result = new PropertyFieldNumeric(_window);
		else if ("1 float" == _type)
			result = new PropertyFieldNumeric(_window);
		else if ("2 float" == _type)
			result = new PropertyFieldNumeric(_window);
		else if ("Alpha" == _type)
			result = new PropertyFieldAlpha(_window);
		else if ("Colour" == _type)
			result = new PropertyFieldEditBox(_window);
		else
			result = new PropertyFieldComboBox(_window);

		result->initialise(_type, _currentWidget);
		return result;
	}

} // namespace tools
