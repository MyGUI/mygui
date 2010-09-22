/*!
	@file
	@author		Ptakhin Alexander
	@date		02/2009
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "MyGUI_FlowContainer.h"

namespace demo
{
	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper() : base::BaseManager() {}

		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

		bool _test(const std::string& _layoutFileName);

		bool _testWidget(MyGUI::Widget* _child, const std::string& _testName);

		MyGUI::IntCoord getFlowWidgetCoords(MyGUI::Widget* _child);

		void runXmlTests();

	private:
		std::vector<MyGUI::Button*> mButtons;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
