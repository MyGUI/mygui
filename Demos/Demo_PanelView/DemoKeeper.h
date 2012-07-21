/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseDemoManager.h"

#include "PanelViewWindow.h"
#include "PanelDirector.h"
#include "PanelDynamic.h"
#include "PanelStatic.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyChangePanels(int _key, size_t _value);
		virtual void setupResources();

	private:
		PanelViewWindow* mView;
		PanelDirector* mPanelDirector;
		PanelDynamic* mPanelDynamic;
		PanelStatic* mPanelStatic;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
