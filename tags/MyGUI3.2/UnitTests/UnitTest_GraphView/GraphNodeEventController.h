/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef __GRAPH_NODE_EVENT_H__
#define __GRAPH_NODE_EVENT_H__

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "EventController.h"

namespace demo
{

	class GraphNodeEventController :
		public BaseAnimationNode
	{
	public:
		GraphNodeEventController(const std::string& _name) :
			BaseAnimationNode("GraphNodeEvent.layout", "EventController", _name),
			mConnectionOut(nullptr),
			mButtonEvent(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());
			assignBase(mConnectionOut, "ConnectionOut");
			assignWidget(mButtonEvent, "ButtonEvent");

			mButtonEvent->eventMouseButtonClick += MyGUI::newDelegate(this, &GraphNodeEventController::notifyMouseButtonClick);
		}

		virtual void shutdown()
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
		wraps::BaseGraphConnection* mConnectionOut;
		MyGUI::Button* mButtonEvent;

	};

} // namespace demo

#endif // __GRAPH_NODE_EVENT_H__
