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
		void addConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin)
		{
			mConnections.emplace_back(_eventout, PairIn(_node, _eventin));
		}

		void removeConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin)
		{
			for (VectorPairOut::iterator item = mConnections.begin(); item != mConnections.end(); ++item)
			{
				if (_eventout == item->first && _node == item->second.first && _eventin == item->second.second)
				{
					mConnections.erase(item);
					return;
				}
			}
			assert(!"connection not found");
		}

		void forceEvent(std::string_view _name, float _value = 0)
		{
			for (auto& mConnection : mConnections)
			{
				if (_name == mConnection.first)
					mConnection.second.first->setEvent(mConnection.second.second, _value);
			}
		}

	private:
		using PairIn = std::pair<IAnimationNode*, std::string>;
		using PairOut = std::pair<std::string, PairIn>;
		using VectorPairOut = std::vector<PairOut>;
		VectorPairOut mConnections;
	};

} // namespace animation

#endif // CONNECTION_RECEIVER_H_
