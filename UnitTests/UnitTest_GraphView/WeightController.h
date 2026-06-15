#ifndef WEIGHT_CONTROLLER_H_
#define WEIGHT_CONTROLLER_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class WeightController : public IAnimationNode
	{
	public:
		WeightController() = default;

		WeightController(std::string_view _name, IAnimationGraph* _graph) :
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
			mConnection.forceEvent("Weight", _value);
		}

	private:
		ConnectionReceiver mConnection;
	};

} // namespace animation

#endif // WEIGHT_CONTROLLER_H_
