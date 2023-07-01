/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_NODE_POSITION_H_
#define GRAPH_NODE_POSITION_H_

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "PositionController.h"

namespace demo
{

	class GraphNodePositionController : public BaseAnimationNode
	{
	public:
		GraphNodePositionController(std::string_view _name) :
			BaseAnimationNode("GraphNodePosition.layout", "PositionController", _name)
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
			mPosition = 0;
			mLength = 1;
			animation::IAnimationNode* node = getAnimationNode()->getGraph()->getNodeByName(mStateName);
			if (node)
			{
				mLength = node->getLength();
				if (mLength > 0.0001f)
					mLength -= 0.0001f;
			}
			else
			{
				mStateName.clear();
			}
			updateWidgets();
		}

		void initialise() override
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());
			assignBase(mConnectionOut, "ConnectionOut");
			assignWidget(mEditPosition, "EditPosition");
			assignWidget(mScrollPosition, "ScrollPosition");

			mEditPosition->eventEditSelectAccept +=
				MyGUI::newDelegate(this, &GraphNodePositionController::notifyEditSelectAccept);
			mScrollPosition->eventScrollChangePosition +=
				MyGUI::newDelegate(this, &GraphNodePositionController::notifyScrollChangePosition);

			updateWidgets();
		}

		void shutdown() override
		{
		}

		void updateWidgets(bool _edit = true, bool _scroll = true)
		{
			if (_edit)
			{
				mEditPosition->setCaption(MyGUI::utility::toString(mPosition));
			}
			if (_scroll)
			{
				double range = (double)mScrollPosition->getScrollRange() - 1;
				mScrollPosition->setScrollPosition((size_t)((range * (double)mPosition / (double)mLength)));
			}

			onChangePosition(mPosition);
		}

		void notifyEditSelectAccept(MyGUI::EditBox* _sender)
		{
			mPosition = MyGUI::utility::parseValue<float>(_sender->getCaption());
			if (mPosition < 0)
				mPosition = 0;
			else if (mPosition > mLength)
				mPosition = mLength;

			updateWidgets();
		}

		void notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
		{
			double range = (double)_sender->getScrollRange() - 1;
			double position = (double)_position;

			mPosition = (float)(position * (double)mLength / range);
			updateWidgets(true, false);
		}

		void onChangePosition(float _position)
		{
			animation::PositionController* controller =
				dynamic_cast<animation::PositionController*>(getAnimationNode());
			if (controller)
				controller->generateEvent(_position);
		}

	private:
		wraps::BaseGraphConnection* mConnectionOut{nullptr};
		MyGUI::EditBox* mEditPosition{nullptr};
		MyGUI::ScrollBar* mScrollPosition{nullptr};
		float mPosition{0.0f};
		float mLength{1.0f};

		std::string mStateName;
	};

} // namespace demo

#endif // GRAPH_NODE_POSITION_H_
