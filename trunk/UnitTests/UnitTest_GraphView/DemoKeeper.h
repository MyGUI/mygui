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
#include "AnimationGraph.h"
#include "AnimationGraphFactory.h"
#include "AnimationNodeFactory.h"

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
		virtual bool frameStarted(const Ogre::FrameEvent& evt);

		void createGrapView();
		void notifyMenuCtrlAccept(wraps::BaseGraphView* _sender, const std::string& _id);
		void notifyNodeClosed(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node);
		void notifyConnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to);
		void notifyDisconnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to);

	private:
		GraphView * mGraphView;

		animation::AnimationGraphFactory* mGraphFactory;
		animation::AnimationNodeFactory* mNodeFactory;
		animation::AnimationGraph* mGraph;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
