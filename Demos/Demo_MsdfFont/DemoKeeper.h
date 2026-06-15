#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include <MyGUI.h>
#include "Base/BaseDemoManager.h"
#include <array>

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void setupResources() override;

		void increaseFontHeight(MyGUI::Widget* _sender);
		void decreaseFontHeight(MyGUI::Widget* _sender);

	private:
		std::array<MyGUI::EditBox*, 5> mEditBoxes;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
