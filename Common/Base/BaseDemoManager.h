/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _95cb35b3_a2e5_471f_93df_56cc381a7449_
#define _95cb35b3_a2e5_471f_93df_56cc381a7449_

#include "BaseManager.h"
#include "StatisticInfo.h"
#include "InputFocusInfo.h"

namespace base
{

	class BaseDemoManager :
		public BaseManager
	{
	public:
		BaseDemoManager() :
			mInfo(nullptr),
			mFocusInfo(nullptr)
		{
		}

		virtual ~BaseDemoManager()
		{
		}

		diagnostic::StatisticInfo* getStatisticInfo()
		{
			return mInfo;
		}

		diagnostic::InputFocusInfo* getFocusInput()
		{
			return mFocusInfo;
		}

		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
		{
			BaseManager::injectKeyPress(_key, _text);

			if (MyGUI::Gui::getInstancePtr() == nullptr)
				return;

			if (_key == MyGUI::KeyCode::F11)
			{
				bool visible = mInfo->getVisible();
				mInfo->setVisible(!visible);
			}
			else if (_key == MyGUI::KeyCode::F12)
			{
				bool visible = mFocusInfo->getFocusVisible();
				mFocusInfo->setFocusVisible(!visible);
			}
		}

		virtual void createScene()
		{
			loadPointerResources();
		}

	protected:
		virtual void createGui()
		{
			BaseManager::createGui();

			mInfo = new diagnostic::StatisticInfo();
			mFocusInfo = new diagnostic::InputFocusInfo();

			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BaseDemoManager::notifyFrameStartUpdateStatistic);
		}

		virtual void destroyGui()
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BaseDemoManager::notifyFrameStartUpdateStatistic);

			if (mInfo)
			{
				delete mInfo;
				mInfo = nullptr;
			}

			if (mFocusInfo)
			{
				delete mFocusInfo;
				mFocusInfo = nullptr;
			}

			BaseManager::destroyGui();
		}

	private:
		void notifyFrameStartUpdateStatistic(float _time)
		{
			if (mInfo != nullptr)
			{
				static float time = 0;
				time += _time;
				if (time > 1)
				{
					time -= 1;

					MyGUI::MapString statistic = getStatistic();
					for (MyGUI::MapString::const_iterator info = statistic.begin(); info != statistic.end(); info ++)
						mInfo->change((*info).first, (*info).second);
					mInfo->update();
				}
			}
		}

	private:
		diagnostic::StatisticInfo* mInfo;
		diagnostic::InputFocusInfo* mFocusInfo;
	};

}

#endif
