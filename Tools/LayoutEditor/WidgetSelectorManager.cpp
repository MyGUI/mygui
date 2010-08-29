/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "WidgetSelectorManager.h"

template <> tools::WidgetSelectorManager* MyGUI::Singleton<tools::WidgetSelectorManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetSelectorManager>::mClassTypeName("WidgetSelectorManager");

namespace tools
{
	WidgetSelectorManager::WidgetSelectorManager() :
		mCurrentWidget(nullptr)
	{
	}

	WidgetSelectorManager::~WidgetSelectorManager()
	{
	}

	void WidgetSelectorManager::initialise()
	{
	}

	void WidgetSelectorManager::shutdown()
	{
	}

	MyGUI::Widget* WidgetSelectorManager::getSelectedWidget()
	{
		return mCurrentWidget;
	}

	void WidgetSelectorManager::setSelectedWidget(MyGUI::Widget* _value)
	{
		// некоторые обновл€ют кей фокус
		//if (_value != mCurrentWidget)
		{
			mCurrentWidget = _value;
			eventChangeSelectedWidget(mCurrentWidget);
		}
	}

} // namespace tools
