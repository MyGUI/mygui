/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "PropertyFieldManager.h"

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

	PropertyField* PropertyFieldManager::createPropertyField(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, MyGUI::Widget* _currentWidget)
	{
		PropertyField* result = new PropertyField();
		result->_create(_window, _property, _value, _type, _currentWidget);
		return result;
	}

} // namespace tools
