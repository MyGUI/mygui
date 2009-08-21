/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_LOOP_CONTROLLER_H__
#define __GRAPH_NODE_LOOP_CONTROLLER_H__

#include <MyGUI.h>
#include "BaseAnimationNode.h"

namespace demo
{

	class GraphNodeLoopController : public BaseAnimationNode
	{
	public:
		GraphNodeLoopController(const std::string& _name) :
			BaseAnimationNode("GraphNodeLoopController.layout"),
			mName(_name),
			mStartIn(nullptr),
			mStopIn(nullptr),
			mWeightIn(nullptr),
			mStartOut(nullptr),
			mStopOut(nullptr),
			mWeightOut(nullptr),
			mPositionOut(nullptr),
			mLength(1)
		{
		}

		virtual void addConnection(const std::string& _eventout, BaseAnimationNode* _node, const std::string& _eventin)
		{
			BaseAnimationNode::addConnection(_eventout, _node, _eventin);

			mLength = _node->getAnimationNode()->getLength();
			//if (mLength > 0.0001) mLength -= 0.0001;

			getAnimationNode()->setProperty("Length", MyGUI::utility::toString(mLength));
		}

	private:
		virtual void initialise()
		{
			mMainWidget->setCaption(mName);

			assignBase(mStartIn, "StartIn");
			assignBase(mStopIn, "StopIn");
			assignBase(mWeightIn, "WeightIn");
			assignBase(mStartOut, "StartOut");
			assignBase(mStopOut, "StopOut");
			assignBase(mWeightOut, "WeightOut");
			assignBase(mPositionOut, "PositionOut");

		}

		virtual void shutdown()
		{
		}

	private:
		std::string mName;
		wraps::BaseGraphConnection* mStartIn;
		wraps::BaseGraphConnection* mStopIn;
		wraps::BaseGraphConnection* mWeightIn;
		wraps::BaseGraphConnection* mStartOut;
		wraps::BaseGraphConnection* mStopOut;
		wraps::BaseGraphConnection* mWeightOut;
		wraps::BaseGraphConnection* mPositionOut;

		float mLength;
	};

} // namespace demo

#endif // __GRAPH_NODE_LOOP_CONTROLLER_H__
