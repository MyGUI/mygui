/*!
	@file
	@author     George Evmenov
	@date       08/2009
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

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

#endif // __DEMO_KEEPER_H__
