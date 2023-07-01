/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"

#include "PanelViewWindow.h"
#include "PanelDirector.h"
#include "PanelDynamic.h"
#include "PanelStatic.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void notifyChangePanels(int _key, size_t _value);
		void setupResources() override;

	private:
		PanelViewWindow* mView{nullptr};
		PanelDirector* mPanelDirector{nullptr};
		PanelDynamic* mPanelDynamic{nullptr};
		PanelStatic* mPanelStatic{nullptr};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
