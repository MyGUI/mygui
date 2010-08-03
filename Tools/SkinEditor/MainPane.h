/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_PANE_H__
#define __MAIN_PANE_H__

#include "BaseManager.h"
#include "BaseLayout/BaseLayout.h"
#include "SkinControl.h"

namespace tools
{

	class MainPane : public wraps::BaseLayout
	{
	public:
		MainPane();
		virtual ~MainPane();

	private:
		SkinControl* mSkinControl;
	};

} // namespace tools

#endif // __MAIN_PANE_H__
