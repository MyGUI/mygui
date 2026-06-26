#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "DataListUI.h"

namespace demo
{
	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		void createScene() override;
		void destroyScene() override;

	private:
		void setupResources() override;

	private:
		DataListUI* mDataListUI;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
