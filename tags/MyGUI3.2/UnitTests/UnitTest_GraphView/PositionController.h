#ifndef __POSITION_CONTROLLER_H__
#define __POSITION_CONTROLLER_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class PositionController :
		public IAnimationNode
	{
	public:
		PositionController()
		{
		}

		PositionController(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph)
		{
		}

		virtual ~PositionController()
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

		void generateEvent(float _value)
		{
			mConnection.forceEvent("Position", _value);
		}

	private:
		ConnectionReceiver mConnection;

	};

} // namespace animation

#endif // __POSITION_CONTROLLER_H__
