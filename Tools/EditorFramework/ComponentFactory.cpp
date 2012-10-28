/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ComponentFactory.h"
#include "FactoryManager.h"
#include "Control.h"
#include "UniqueNamePropertyInitialisator.h"
#include "FocusInfoControl.h"
#include "DataListBaseControl.h"
#include "ColourPanel.h"
#include "ChangeValueAction.h"
#include "BackgroundControl.h"
#include "ActionRenameData.h"
#include "ActionChangeDataProperty.h"
#include "TexturePropertyInitialisator.h"
#include "TextureBrowseControl.h"
#include "TextFieldControl.h"
#include "SettingsWindow.h"
#include "ScopeTextureControl.h"
#include "PropertyTexturesControl.h"
#include "PropertyStringControl.h"
#include "PropertyPanelController.h"
#include "PropertyPanelControl.h"
#include "PropertyIntControl.h"
#include "PropertyInt4Control.h"
#include "PropertyInt2Control.h"
#include "PropertyFloatControl.h"
#include "PropertyColourControl.h"
#include "PropertyBoolControl.h"
#include "OpenSaveFileDialog.h"
#include "MessageBoxFadeControl.h"
#include "MainMenuControl.h"
#include "ListBoxDataControl.h"
#include "SeparatePanel.h"

namespace tools
{

	void ComponentFactory::Initialise()
	{
		FACTORY_ITEM(Control)
		FACTORY_ITEM(FocusInfoControl)
		FACTORY_ITEM(DataListBaseControl)
		FACTORY_ITEM(ColourPanel)
		FACTORY_ITEM(BackgroundControl)
		FACTORY_ITEM(TextureBrowseControl)
		FACTORY_ITEM(TextFieldControl)
		FACTORY_ITEM(SettingsWindow)
		FACTORY_ITEM(ScopeTextureControl)
		FACTORY_ITEM(PropertyTexturesControl)
		FACTORY_ITEM(PropertyStringControl)
		FACTORY_ITEM(PropertyPanelControl)
		FACTORY_ITEM(PropertyIntControl)
		FACTORY_ITEM(PropertyInt4Control)
		FACTORY_ITEM(PropertyInt2Control)
		FACTORY_ITEM(PropertyFloatControl)
		FACTORY_ITEM(PropertyColourControl)
		FACTORY_ITEM(PropertyBoolControl)
		FACTORY_ITEM(OpenSaveFileDialog)
		FACTORY_ITEM(MessageBoxFadeControl)
		FACTORY_ITEM(MainMenuControl)
		FACTORY_ITEM(ListBoxDataControl)
		FACTORY_ITEM(SeparatePanel)

		FACTORY_ITEM(PropertyPanelController)

		FACTORY_ITEM(UniqueNamePropertyInitialisator)
		FACTORY_ITEM(TexturePropertyInitialisator)

		FACTORY_ITEM(ChangeValueAction)
		FACTORY_ITEM(ActionRenameData)
		FACTORY_ITEM(ActionChangeDataProperty)
	}

	void ComponentFactory::Shutdown()
	{
		::components::FactoryManager::GetInstancePtr()->UnregisterAllFactories();
	}
}
