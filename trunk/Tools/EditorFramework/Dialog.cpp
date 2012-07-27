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
		mModal(false),
		mRootWidget(nullptr)
	{
	}

	Dialog::~Dialog()
	{
	}

	void Dialog::doModal()
	{
		MYGUI_ASSERT(mModal != true, "Already modal mode");
		mModal = true;

		MyGUI::InputManager::getInstance().addWidgetModal(mRootWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mRootWidget);

		onDoModal();

		mRootWidget->setVisible(true);

		DialogManager::getInstance()._addDialog(this);
	}

	void Dialog::endModal()
	{
		MYGUI_ASSERT(mModal != false, "Already modal mode");
		mModal = false;

		mRootWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mRootWidget);
		DialogManager::getInstance()._removeDialog(this);

		onEndModal();
	}

	void Dialog::setDialogRoot(MyGUI::Widget* _root)
	{
		mRootWidget = _root;
	}

	bool Dialog::isDialogModal()
	{
		return mModal;
	}

}
