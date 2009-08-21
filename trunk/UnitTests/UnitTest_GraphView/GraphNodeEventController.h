/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_EVENT_H__
#define __GRAPH_NODE_EVENT_H__

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "EventController.h"

namespace demo
{

	class GraphNodeEventController : public BaseAnimationNode
	{
	public:
		GraphNodeEventController(const std::string& _name) :
			BaseAnimationNode("GraphNodeEvent.layout"),
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

			mButtonEvent->eventMouseButtonClick = MyGUI::newDelegate(this, &GraphNodeEventController::notifyMouseButtonClick);
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
			animation::EventController* controller = dynamic_cast<animation::EventController*>(getAnimationNode());
			if (controller)
				controller->generateEvent();
		}

	private:
		std::string mName;
		wraps::BaseGraphConnection * mConnectionOut;
		MyGUI::ButtonPtr mButtonEvent;

	};

} // namespace demo

#endif // __GRAPH_NODE_EVENT_H__
