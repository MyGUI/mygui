/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "MessageBoxFadeControl.h"
#include "MessageBoxManager.h"

namespace tools
{

	MessageBoxFadeControl::MessageBoxFadeControl() :
		mMaxAlpha(1)
	{
	}

	MessageBoxFadeControl::~MessageBoxFadeControl()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &MessageBoxFadeControl::notifyFrameStart);
	}

	void MessageBoxFadeControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "MessageBoxFadeControl.layout");

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &MessageBoxFadeControl::notifyFrameStart);

		mMaxAlpha = mMainWidget->getAlpha();
		mMainWidget->setAlpha(0);
	}


	void MessageBoxFadeControl::notifyFrameStart(float _time)
	{
		const float coef = 1;

		bool visible = MessageBoxManager::getInstance().hasAny();

		if (visible)
		{
			if (!mMainWidget->getVisible())
			{
				mMainWidget->setVisible(true);
				mMainWidget->setAlpha(0);
			}
			else
			{
				float alpha = mMainWidget->getAlpha();
				if (alpha < mMaxAlpha)
				{
					alpha += _time * coef;
					if (alpha > mMaxAlpha)
						alpha = mMaxAlpha;
					mMainWidget->setAlpha(alpha);
				}
			}
		}
		else
		{
			if (mMainWidget->getVisible())
			{
				float alpha = mMainWidget->getAlpha();
				alpha -= _time * coef;
				if (alpha <= 0)
				{
					mMainWidget->setVisible(false);
				}
				else
				{
					mMainWidget->setAlpha(alpha);
				}
			}
		}
	}

}
