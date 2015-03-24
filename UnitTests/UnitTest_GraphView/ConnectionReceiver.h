/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef CONNECTION_RECEIVER_H_
#define CONNECTION_RECEIVER_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"

namespace animation
{

	class ConnectionReceiver
	{
	public:
		ConnectionReceiver()
		{
		}

		~ConnectionReceiver()
		{
		}

		void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnections.push_back(PairOut(_eventout, PairIn(_node, _eventin)));
		}

		void removeConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			for (VectorPairOut::iterator item = mConnections.begin(); item != mConnections.end(); ++item)
			{
				if (_eventout == item->first
					&& _node == item->second.first
					&& _eventin == item->second.second)
				{
					mConnections.erase(item);
					return;
				}
			}
			assert(!"connection not found");
		}

		void forceEvent(const std::string& _name, float _value = 0)
		{
			for (VectorPairOut::iterator item = mConnections.begin(); item != mConnections.end(); ++item)
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

#endif // CONNECTION_RECEIVER_H_
