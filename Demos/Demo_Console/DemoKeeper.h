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

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;

	private:
		void command(const MyGUI::UString& _key, const MyGUI::UString& _value);
		void setupResources() override;

	private:
		Console* mConsole{nullptr};
		MyGUI::EditBox* mEdit{nullptr};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
