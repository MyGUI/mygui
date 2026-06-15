#ifndef POSITION_CONTROLLER_H_
#define POSITION_CONTROLLER_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class PositionController : public IAnimationNode
	{
	public:
		PositionController() = default;

		PositionController(std::string_view _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph)
		{
		}

		void addConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin) override
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		void removeConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin) override
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

#endif // POSITION_CONTROLLER_H_
