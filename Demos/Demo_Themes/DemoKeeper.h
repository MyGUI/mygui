/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void createDemo(size_t _index);
		void destroyDemo();
		virtual void setupResources();

		void colourWidgets(MyGUI::Widget* _widget, const MyGUI::Colour& _colour);

	private:
		MyGUI::Widget* mDemoView;
		MyGUI::ComboBox* mComboSkins;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
