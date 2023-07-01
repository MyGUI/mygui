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

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void createDemo(size_t _index);
		void destroyDemo();
		void setupResources() override;

		void colourWidgets(MyGUI::Widget* _widget, const MyGUI::Colour& _colour);

	private:
		MyGUI::Widget* mDemoView{nullptr};
		MyGUI::ComboBox* mComboSkins{nullptr};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
