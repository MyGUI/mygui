#ifndef DEMO_WIDGET_ROTATION_KEEPER_H_
#define DEMO_WIDGET_ROTATION_KEEPER_H_

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;

	private:
		void setupResources() override;
		void notifyButtonClick(MyGUI::Widget* _sender);
		MyGUI::Window* mWindow;
	};

}

#endif
