#ifndef WEIGHT_CONTROLLER_H_
#define WEIGHT_CONTROLLER_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class WeightController :
		public IAnimationNode
	{
	public:
		WeightController()
		{
		}

		WeightController(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph)
		{
		}

		virtual ~WeightController()
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
			mConnection.forceEvent("Weight", _value);
		}

	private:
		ConnectionReceiver mConnection;

	};

} // namespace animation

#endif // WEIGHT_CONTROLLER_H_
