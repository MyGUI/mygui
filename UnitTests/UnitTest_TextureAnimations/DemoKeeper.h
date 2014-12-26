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

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		void setupResources();
		void createNewWindow();
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

	private:
		typedef std::set<MyGUI::Window*> SetWidget;
		SetWidget mWidgets;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
