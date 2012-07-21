/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "StatisticInfoControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "SettingsManager.h"
#include "Application.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(StatisticInfoControl)

	StatisticInfoControl::StatisticInfoControl() :
		mText(nullptr)
	{
	}

	StatisticInfoControl::~StatisticInfoControl()
	{
	}

	void StatisticInfoControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "StatisticInfoControl.layout");

		assignWidget(mText, "Text");

		CommandManager::getInstance().getEvent("Command_StatisticInfo")->connect(this, &StatisticInfoControl::command_StatisticInfo);

		getRoot()->setVisible(SettingsManager::getInstance().getValue<bool>("Editor/Controls/StatisticInfoControl/Visible"));

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &StatisticInfoControl::notifyFrameStart);
	}

	void StatisticInfoControl::command_StatisticInfo(const MyGUI::UString& _commandName, bool& _result)
	{
		getRoot()->setVisible(!getRoot()->getVisible());
		SettingsManager::getInstance().setValue("Editor/Controls/StatisticInfoControl/Visible", getRoot()->getVisible());

		_result = true;
	}

	void StatisticInfoControl::notifyFrameStart(float _time)
	{
		static float time = 0;
		time += _time;
		if (time > 1)
		{
			time -= 1;

			std::string value;

			base::BaseManager::MapString statistic = Application::getInstance().getStatistic();
			for (base::BaseManager::MapString::const_iterator info = statistic.begin(); info != statistic.end(); info ++)
			{
				if (!value.empty())
					value += "\n";
				value += (*info).first + " : " + (*info).second;
			}

			mText->setCaption(value);
		}
	}

}
