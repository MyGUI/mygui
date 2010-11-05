/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_PANE_H__
#define __MAIN_PANE_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinControl.h"
#include "StateControl.h"
#include "SeparatorControl.h"
#include "RegionControl.h"
#include "MainMenuControl.h"

namespace tools
{

	class MainPane :
		public wraps::BaseLayout
	{
	public:
		MainPane();
		virtual ~MainPane();

		void setVisible(bool _value);

	private:
		void notifyTabChangeSelect(MyGUI::Tab* _sender, size_t _index);

	private:
		SkinControl* mSkinControl;
		StatesControl* mStatesControl;
		SeparatorControl* mSeparatorControl;
		RegionControl* mRegionControl;
		MainMenuControl* mMainMenuControl;

		MyGUI::Tab* mTab;
	};

} // namespace tools

#endif // __MAIN_PANE_H__
