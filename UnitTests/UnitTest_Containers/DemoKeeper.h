/*!
	@file
	@author		Ptakhin Alexander
	@date		02/2009
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "MyGUI_FlowContainer.h"

namespace demo
{
	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper() : base::BaseManager() {}

		virtual void createScene();

	private:
		virtual void setupResources();

		bool _test(const std::string& _layoutFileName);

		bool _testWidget(MyGUI::Widget* _child, const std::string& _testName);

		MyGUI::IntCoord getFlowWidgetCoords(MyGUI::Widget* _child);

		void runXmlTests();

	private:
		MyGUI::FlowContainerPtr mFlow;

		std::vector<MyGUI::ButtonPtr> mButtons;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
