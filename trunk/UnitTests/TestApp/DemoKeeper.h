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
		virtual void setupResources();

		void addText(MyGUI::Widget* _parent);
		void addImage(MyGUI::Widget* _parent);

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
