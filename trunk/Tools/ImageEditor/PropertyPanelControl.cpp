/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "PropertyPanelControl.h"
#include "FactoryManager.h"
#include "ScopeManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyPanelControl)

	PropertyPanelControl::PropertyPanelControl()
	{
	}

	PropertyPanelControl::~PropertyPanelControl()
	{
	}

	void PropertyPanelControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		ScopeManager::getInstance().eventChangeScope.connect(this, &PropertyPanelControl::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void PropertyPanelControl::notifyChangeScope(const std::string& _scope)
	{
	}

}
