/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FontTryControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "SettingsManager.h"
//#include "Application.h"
//#include "ScopeManager.h"
#include "DataManager.h"
#include "DataUtility.h"
#include "DataSelectorManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FontTryControl)

	FontTryControl::FontTryControl() :
		mEdit(nullptr)
	{
	}

	FontTryControl::~FontTryControl()
	{
		DataSelectorManager::getInstance().getEvent("Root")->disconnect(this);

		SettingsManager::getInstance().setValue("Controls/TryFontControl/Text", mEdit->getCaption());
	}

	void FontTryControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		mEdit = mMainWidget->castType<MyGUI::EditBox>();

		mEdit->setCaption(SettingsManager::getInstance().getValue("Controls/TryFontControl/Text"));
		
		CommandManager::getInstance().getEvent("Command_OnGenerateFont")->connect(this, &FontTryControl::commandOnGenerateFont);
		DataSelectorManager::getInstance().getEvent("Root")->connect(this, &FontTryControl::notifyChangeDataSelector);
		updateFont();
	}

	void FontTryControl::commandOnGenerateFont(const MyGUI::UString& _commandName, bool& _result)
	{
		updateFont();
	}

	void FontTryControl::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		updateFont();
	}

	void FontTryControl::updateFont()
	{
		DataPtr data = DataUtility::getSelectedDataByType("Font");
		if (data != nullptr)
			mEdit->setFontName(data->getPropertyValue("FontName"));
		else
			mEdit->setFontName("");
	}

}
