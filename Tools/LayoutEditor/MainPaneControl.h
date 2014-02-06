#ifndef _c094d4c6_6c07_414d_979a_f85dde3f4e7b_
#define _c094d4c6_6c07_414d_979a_f85dde3f4e7b_

#include "BaseLayout/BaseLayout.h"
#include "MainMenuControl.h"
#include "MainWorkspaceControl.h"

namespace tools
{

	class MainPaneControl :
		public wraps::BaseLayout
	{
	public:
		MainPaneControl();
		virtual ~MainPaneControl();

		void setVisible(bool _value);

	private:
		MainMenuControlLE* mMainMenuControl;
		MainWorkspaceControl* mMainWorkspaceControl;
	};

}

#endif
