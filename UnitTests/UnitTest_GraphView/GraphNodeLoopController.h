/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_NODE_LOOP_CONTROLLER_H_
#define GRAPH_NODE_LOOP_CONTROLLER_H_

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "IAnimationGraph.h"

namespace demo
{

	class GraphNodeLoopController : public BaseAnimationNode
	{
	public:
		GraphNodeLoopController(std::string_view _name) :
			BaseAnimationNode("GraphNodeLoopController.layout", "LoopController", _name)
		{
		}

		void addConnection(std::string_view _eventout, BaseAnimationNode* _node, std::string_view _eventin) override
		{
			BaseAnimationNode::addConnection(_eventout, _node, _eventin);
			if (_eventout == "Position")
			{
				mStateName = _node->getAnimationNode()->getName();
				updateStateLenght();
			}
		}

		void removeConnection(std::string_view _eventout, BaseAnimationNode* _node, std::string_view _eventin) override
		{
			BaseAnimationNode::removeConnection(_eventout, _node, _eventin);
			if (_eventout == "Position")
			{
				mStateName.clear();
				updateStateLenght();
			}
		}

		void invalidateNode(BaseAnimationNode* _sender) override
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

		void initialise() override
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

		void shutdown() override
		{
		}

	private:
		wraps::BaseGraphConnection* mStartIn{nullptr};
		wraps::BaseGraphConnection* mStopIn{nullptr};
		wraps::BaseGraphConnection* mWeightIn{nullptr};
		wraps::BaseGraphConnection* mStartOut{nullptr};
		wraps::BaseGraphConnection* mStopOut{nullptr};
		wraps::BaseGraphConnection* mWeightOut{nullptr};
		wraps::BaseGraphConnection* mPositionOut{nullptr};

		float mLength{1};
		std::string mStateName;
	};

} // namespace demo

#endif // GRAPH_NODE_LOOP_CONTROLLER_H_
