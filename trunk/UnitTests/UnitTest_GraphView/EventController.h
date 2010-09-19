#ifndef __EVENT_CONTROLLER_H__
#define __EVENT_CONTROLLER_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class EventController :
		public IAnimationNode
	{
	public:
		EventController()
		{
		}

		EventController(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph)
		{
		}

		virtual ~EventController()
		{
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		virtual void removeConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnection.removeConnection(_eventout, _node, _eventin);
		}

		void generateEvent()
		{
			mConnection.forceEvent("Event");
		}

	private:
		ConnectionReceiver mConnection;

	};

} // namespace animation

#endif // __EVENT_CONTROLLER_H__
