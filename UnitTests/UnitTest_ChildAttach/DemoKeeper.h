/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
		virtual void setupResources();

	private:

		void notifyMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);
		void notifyMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

		void createSuccesScene();
		void createFailedScene();

		MyGUI::TextBox* mTextBoxTest;

	};

} // namespace demo

#endif // DEMO_KEEPER_H_
