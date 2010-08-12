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

	private:
		SkinControl* mSkinControl;
		StatesControl* mStatesControl;
		SeparatorControl* mSeparatorControl;
		RegionControl* mRegionControl;
		MainMenuControl* mMainMenuControl;
	};

} // namespace tools

#endif // __MAIN_PANE_H__
