/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
#include "precompiled.h"
#include "EnemyPanel.h"

namespace demo
{

	EnemyPanel::EnemyPanel()
	{
		initialiseByAttributes(this);

		const MyGUI::IntSize& size = MyGUI::Gui::getInstance().getViewSize();
		int offset = size.width / 3;

		mMainWidget->setPosition(offset + offset - (mMainWidget->getWidth() / 2), (size.height - mMainWidget->getHeight()) / 2);
	}


} // namespace demo
