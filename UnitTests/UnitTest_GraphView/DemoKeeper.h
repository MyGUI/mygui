/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "GraphView.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper();


	private:
		virtual void createScene();
		virtual void destroyScene();
		virtual void setupResources();

		void createGrapView();
		void notifyMenuCtrlAccept(wraps::BaseGraphView* _sender, const std::string& _id);
		void notifyNodeClosed(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node);

	private:
		GraphView * mGraphView;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
