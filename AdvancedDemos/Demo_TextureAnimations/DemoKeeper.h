/*!
	@file
	@author     George Evmenov
	@date       08/2009
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;
		void setupResources() override;
		void createNewWindow();
		void notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name);

	private:
		using SetWidget = std::set<MyGUI::Window*>;
		SetWidget mWidgets;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
