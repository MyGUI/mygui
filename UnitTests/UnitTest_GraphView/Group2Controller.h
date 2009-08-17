#ifndef __GROUP_2_CONTROLLER_H__
#define __GROUP_2_CONTROLLER_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"

namespace animation
{

	class Group2Controller : public IAnimationNode
	{
	public:
		Group2Controller()
		{
		}

		Group2Controller(const std::string& _name, IAnimationGraph* _holder) :
		IAnimationNode(_name)
		{
		}

		virtual ~Group2Controller()
		{
		}

		virtual void setEvent(const std::string& _name, float _value)
		{
			if (_name == "Start1") startAnimation1();
			else if (_name == "Start2") startAnimation2();
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnections.push_back(PairOut(_eventout, PairIn(_node, _eventin)));
		}

	private:
		void startAnimation1()
		{
			forceEvent("Stop2");
			forceEvent("Start1");
		}

		void startAnimation2()
		{
			forceEvent("Stop1");
			forceEvent("Start2");
		}

		void forceEvent(const std::string& _name, float _value = 0)
		{
			for (VectorPairOut::iterator item=mConnections.begin(); item!=mConnections.end(); ++item)
			{
				if (_name == item->first)
					item->second.first->setEvent(item->second.second, _value);
			}
		}

	private:
		typedef std::pair<IAnimationNode*, std::string> PairIn;
		typedef std::pair<std::string, PairIn> PairOut;
		typedef std::vector<PairOut> VectorPairOut;
		VectorPairOut mConnections;

	};

} // namespace animation

#endif // __GROUP_2_CONTROLLER_H__
