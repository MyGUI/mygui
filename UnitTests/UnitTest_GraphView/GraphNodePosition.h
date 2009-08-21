/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_POSITION_H__
#define __GRAPH_NODE_POSITION_H__

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "PositionController.h"

namespace demo
{

	class GraphNodePosition : public BaseAnimationNode
	{
	public:
		GraphNodePosition(const std::string& _name) :
			BaseAnimationNode("GraphNodePosition.layout"),
			mName(_name),
			mConnectionOut(nullptr),
			mPosition(0),
			mLength(1)
		{
		}

		virtual void addConnection(const std::string& _eventout, BaseAnimationNode* _node, const std::string& _eventin)
		{
			BaseAnimationNode::addConnection(_eventout, _node, _eventin);
			mLength = _node->getAnimationNode()->getLength();
			if (mLength > 0.0001) mLength -= 0.0001;
			mPosition = 0;
			updateWidgets();
		}

	private:
		virtual void initialise()
		{
			mMainWidget->setCaption(mName);
			assignBase(mConnectionOut, "ConnectionOut");
			assignWidget(mEditPosition, "EditPosition");
			assignWidget(mScrollPosition, "ScrollPosition");

			mEditPosition->eventEditSelectAccept = MyGUI::newDelegate(this, &GraphNodePosition::notifyEditSelectAccept);
			mScrollPosition->eventScrollChangePosition = MyGUI::newDelegate(this, &GraphNodePosition::notifyScrollChangePosition);

			updateWidgets();
		}

		virtual void shutdown()
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

		void notifyEditSelectAccept(MyGUI::EditPtr _sender)
		{
			mPosition = MyGUI::utility::parseValue<float>(_sender->getCaption());
			if (mPosition < 0) mPosition = 0;
			else if (mPosition > mLength) mPosition = mLength;

			updateWidgets();
		}

		void notifyScrollChangePosition(MyGUI::VScrollPtr _sender, size_t _position)
		{
			double range = (double)_sender->getScrollRange() - 1;
			double position = (double)_position;

			mPosition = (float)(position * (double)mLength / range);
			updateWidgets(true, false);
		}

		void onChangePosition(float _position)
		{
			animation::PositionController* controller = dynamic_cast<animation::PositionController*>(getAnimationNode());
			if (controller)
				controller->generateEvent(_position);
		}

	private:
		std::string mName;
		wraps::BaseGraphConnection * mConnectionOut;
		MyGUI::EditPtr mEditPosition;
		MyGUI::HScrollPtr mScrollPosition;
		float mPosition;
		float mLength;

	};

} // namespace demo

#endif // __GRAPH_NODE_POSITION_H__
