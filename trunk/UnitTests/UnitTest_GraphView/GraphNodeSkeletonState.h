/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_SKELETON_STATE_H__
#define __GRAPH_NODE_SKELETON_STATE_H__

#include <MyGUI.h>
#include "BaseGraphNode.h"

namespace demo
{

	class GraphNodeSkeletonState : public wraps::BaseGraphNode
	{
	public:
		GraphNodeSkeletonState(const std::string& _name) :
			BaseGraphNode("GraphNodeSkeletonState.layout"),
			mName(_name),
			mStartIn(nullptr),
			mStopIn(nullptr),
			mPositionIn(nullptr),
			mWeightIn(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->setCaption(mName);
			assignBase(mStartIn, "StartIn");
			assignBase(mStopIn, "StopIn");
			assignBase(mPositionIn, "PositionIn");
			assignBase(mWeightIn, "WeightIn");
			//assignWidget(mButtonEvent, "ButtonEvent");

			//mButtonEvent->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyMouseButtonClick);
		}

		virtual void shutdown()
		{
		}

		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
		{
			onEvent();
		}

		void onEvent()
		{
		}

	private:
		std::string mName;
		wraps::BaseGraphConnection * mStartIn;
		wraps::BaseGraphConnection * mStopIn;
		wraps::BaseGraphConnection * mPositionIn;
		wraps::BaseGraphConnection * mWeightIn;
		//MyGUI::ButtonPtr mButtonEvent;

	};

} // namespace demo

#endif // __GRAPH_NODE_SKELETON_STATE_H__
