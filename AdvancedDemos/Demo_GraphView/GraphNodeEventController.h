/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_NODE_EVENT_H_
#define GRAPH_NODE_EVENT_H_

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "EventController.h"

namespace demo
{

	class GraphNodeEventController : public BaseAnimationNode
	{
	public:
		GraphNodeEventController(std::string_view _name) :
			BaseAnimationNode("GraphNodeEvent.layout", "EventController", _name)
		{
		}

	private:
		void initialise() override
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());
			assignBase(mConnectionOut, "ConnectionOut");
			assignWidget(mButtonEvent, "ButtonEvent");

			mButtonEvent->eventMouseButtonClick +=
				MyGUI::newDelegate(this, &GraphNodeEventController::notifyMouseButtonClick);
		}

		void shutdown() override
		{
		}

		void notifyMouseButtonClick(MyGUI::Widget* _sender)
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
		wraps::BaseGraphConnection* mConnectionOut{nullptr};
		MyGUI::Button* mButtonEvent{nullptr};
	};

} // namespace demo

#endif // GRAPH_NODE_EVENT_H_
