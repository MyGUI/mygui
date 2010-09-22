/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#include "Precompiled.h"
#include "EnemyPanel.h"

namespace demo
{

	EnemyPanel::EnemyPanel()
	{
		initialiseByAttributes(this);

		const MyGUI::IntSize& size = mMainWidget->getParentSize();
		int offset = size.width / 3;

		mMainWidget->setPosition(offset + offset - (mMainWidget->getWidth() / 2), (size.height - mMainWidget->getHeight()) / 2);
	}

	bool EnemyPanel::isIntersect(int _x, int _y)
	{
		return mMainWidget->getAbsoluteRect().inside(MyGUI::IntPoint(_x, _y));
	}

	void EnemyPanel::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

} // namespace demo
