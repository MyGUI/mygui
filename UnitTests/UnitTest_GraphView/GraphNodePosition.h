/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_POSITION_H__
#define __GRAPH_NODE_POSITION_H__

#include <MyGUI.h>
#include "BaseGraphNode.h"

namespace demo
{

	class GraphNodePosition : public wraps::BaseGraphNode
	{
	public:
		GraphNodePosition(const std::string& _name) :
			BaseGraphNode("GraphNodePosition.layout"),
			mName(_name),
			mConnectionOut(nullptr),
			mPosition(0)
		{
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
				mEditPosition->setCaption(MyGUI::utility::toString(mPosition));
			if (_scroll)
			{
				double range = (double)mScrollPosition->getScrollRange() - 1;
				mScrollPosition->setScrollPosition((size_t)((range * (double)mPosition)));
			}

			onChangePosition(mPosition);
		}

		void notifyEditSelectAccept(MyGUI::EditPtr _sender)
		{
			mPosition = MyGUI::utility::parseValue<float>(_sender->getCaption());
			if (mPosition < 0) mPosition = 0;
			else if (mPosition > 1) mPosition = 1;

			updateWidgets();
		}

		void notifyScrollChangePosition(MyGUI::VScrollPtr _sender, size_t _position)
		{
			double range = (double)_sender->getScrollRange() - 1;
			double position = (double)_position;

			mPosition = (float)(position / range);
			updateWidgets(true, false);
		}

		void onChangePosition(float _position)
		{
		}

	private:
		std::string mName;
		wraps::BaseGraphConnection * mConnectionOut;
		MyGUI::EditPtr mEditPosition;
		MyGUI::HScrollPtr mScrollPosition;
		float mPosition;

	};

} // namespace demo

#endif // __GRAPH_NODE_POSITION_H__
