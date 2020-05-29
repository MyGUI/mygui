#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include <MyGUI.h>
#include "Base/BaseDemoManager.h"
#include <array>

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

		void increaseFontHeight(MyGUI::Widget* _sender);
		void decreaseFontHeight(MyGUI::Widget* _sender);
	private:
		std::array<MyGUI::EditBox*, 5> mEditBoxes;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
