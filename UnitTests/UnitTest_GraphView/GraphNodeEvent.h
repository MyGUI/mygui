/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_EVENT_H__
#define __GRAPH_NODE_EVENT_H__

#include <MyGUI.h>
#include "BaseGraphNode.h"

namespace demo
{

	class GraphNodeEvent : public wraps::BaseGraphNode
	{
	public:
		GraphNodeEvent(const std::string& _name) :
			BaseGraphNode("GraphNodeEvent.layout"),
			mName(_name),
			mConnectionOut(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->setCaption(mName);
			assignBase(mConnectionOut, "ConnectionOut");
			assignWidget(mButtonEvent, "ButtonEvent");

			mButtonEvent->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphNodeEvent::notifyMouseButtonClick);
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
		wraps::BaseGraphConnection * mConnectionOut;
		MyGUI::ButtonPtr mButtonEvent;

	};

} // namespace demo

#endif // __GRAPH_NODE_EVENT_H__
