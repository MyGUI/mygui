/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include <MyGUI.h>
#include "Base/BaseDemoManager.h"
#include "Console.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

	private:
		void command(const MyGUI::UString& _key, const MyGUI::UString& _value);
		virtual void setupResources();

	private:
		Console* mConsole;
		MyGUI::EditBox* mEdit;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
