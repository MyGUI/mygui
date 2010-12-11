/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef __GRAPH_NODE_LOOP_CONTROLLER_H__
#define __GRAPH_NODE_LOOP_CONTROLLER_H__

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "IAnimationGraph.h"

namespace demo
{

	class GraphNodeLoopController :
		public BaseAnimationNode
	{
	public:
		GraphNodeLoopController(const std::string& _name) :
			BaseAnimationNode("GraphNodeLoopController.layout", "LoopController", _name),
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
			if (_eventout == "Position")
			{
				mStateName = _node->getAnimationNode()->getName();
				updateStateLenght();
			}
		}

		virtual void removeConnection(const std::string& _eventout, BaseAnimationNode* _node, const std::string& _eventin)
		{
			BaseAnimationNode::removeConnection(_eventout, _node, _eventin);
			if (_eventout == "Position")
			{
				mStateName.clear();
				updateStateLenght();
			}
		}

		virtual void invalidateNode(BaseAnimationNode* _sender)
		{
			BaseAnimationNode::invalidateNode(_sender);
			if (_sender->getAnimationNode()->getName() == mStateName)
			{
				updateStateLenght();
			}
		}

	private:
		void updateStateLenght()
		{
			mLength = 1;
			animation::IAnimationNode* node = getAnimationNode()->getGraph()->getNodeByName(mStateName);
			if (node)
			{
				mLength = node->getLength();
				//if (mLength > 0.0001) mLength -= 0.0001;
			}
			else
			{
				mStateName.clear();
			}

			getAnimationNode()->setProperty("Length", MyGUI::utility::toString(mLength));
		}

		virtual void initialise()
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());

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
		wraps::BaseGraphConnection* mStartIn;
		wraps::BaseGraphConnection* mStopIn;
		wraps::BaseGraphConnection* mWeightIn;
		wraps::BaseGraphConnection* mStartOut;
		wraps::BaseGraphConnection* mStopOut;
		wraps::BaseGraphConnection* mWeightOut;
		wraps::BaseGraphConnection* mPositionOut;

		float mLength;
		std::string mStateName;
	};

} // namespace demo

#endif // __GRAPH_NODE_LOOP_CONTROLLER_H__
