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
//#include "SkinListControl.h"

namespace tools
{

	class MainPane :
		public Control
	{
	public:
		MainPane();
		virtual ~MainPane();

		void setVisible(bool _value);

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index);

	private:
		//SkinControl* mSkinControl;
		//StatesControl* mStatesControl;
		//SeparatorControl* mSeparatorControl;
		//RegionControl* mRegionControl;
		//SkinListControl* mSkinListControl;
		MainMenuControl* mMainMenuControl;

		MyGUI::TabControl* mTab;
	};

} // namespace tools

#endif // __MAIN_PANE_H__
