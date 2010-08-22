/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MessageBoxFadeControl.h"
#include "MessageBoxManager.h"

namespace tools
{

	MessageBoxFadeControl::MessageBoxFadeControl() :
		wraps::BaseLayout("MessageBoxFadeControl.layout")
	{
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &MessageBoxFadeControl::notifyFrameStart);
	}

	MessageBoxFadeControl::~MessageBoxFadeControl()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &MessageBoxFadeControl::notifyFrameStart);
	}

	void MessageBoxFadeControl::notifyFrameStart(float _time)
	{
		bool visible = MessageBoxManager::getInstance().hasAny();
		if (mMainWidget->getVisible() != visible)
			mMainWidget->setVisible(visible);
	}

} // namespace tools
