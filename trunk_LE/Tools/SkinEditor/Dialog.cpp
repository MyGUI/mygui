/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "Dialog.h"
#include "DialogManager.h"

namespace tools
{

	Dialog::Dialog() :
		wraps::BaseLayout(),
		mModal(false)
	{
	}

	Dialog::Dialog(const std::string& _layout) :
		wraps::BaseLayout(_layout),
		mModal(false)
	{
	}

	Dialog::~Dialog()
	{
	}

	void Dialog::doModal()
	{
		MYGUI_ASSERT(mModal != true, "Already modal mode");
		mModal = true;

		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		onDoModal();

		mMainWidget->setVisible(true);

		DialogManager::getInstance()._addDialog(this);
	}

	void Dialog::endModal()
	{
		MYGUI_ASSERT(mModal != false, "Already modal mode");
		mModal = false;

		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
		DialogManager::getInstance()._removeDialog(this);

		onEndModal();
	}

} // namespace tools
