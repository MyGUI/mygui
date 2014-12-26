#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "DataListUI.h"

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
		DataListUI* mDataListUI;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
