/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index);
		void createDemo(size_t _index);
		void destroyDemo();

	private:
		MyGUI::WidgetPtr mDemoView;
		MyGUI::ComboBoxPtr mComboSkins;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
