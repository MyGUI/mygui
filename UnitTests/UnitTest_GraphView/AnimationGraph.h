/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef ANIMATION_GRAPH_H_
#define ANIMATION_GRAPH_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class AnimationGraph : public IAnimationGraph
	{
	public:
		AnimationGraph() = default;

		AnimationGraph(std::string_view _name) :
			IAnimationGraph(_name)
		{
		}

		void setEvent(std::string_view _name, float _value = 0) override
		{
			mConnection.forceEvent(_name, _value);
		}

		void addConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin) override
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		void removeConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin) override
		{
			mConnection.removeConnection(_eventout, _node, _eventin);
		}

		void addTime(float _value) override
		{
			for (auto& mNode : mNodes)
			{
				mNode->addTime(_value);
			}
		}

		void addNode(IAnimationNode* _node) override
		{
			mNodes.push_back(_node);
		}

		void removeNode(IAnimationNode* _node) override
		{
			VectorNode::iterator item = std::find(mNodes.begin(), mNodes.end(), _node);
			assert(item != mNodes.end());
			mNodes.erase(item);
		}

		IAnimationNode* getNodeByName(std::string_view _name) override
		{
			if (_name == getName())
				return this;

			for (auto& mNode : mNodes)
			{
				if (mNode->getName() == _name)
				{
					return mNode;
				}
			}
			return nullptr;
		}

		Ogre::Any getData(std::string_view _name) override
		{
			MapAny::iterator item = mDatas.find(_name);
			if (item != mDatas.end())
				return item->second;
			return {};
		}

		void addData(std::string_view _name, Ogre::Any _any) override
		{
			MyGUI::mapSet(mDatas, _name, _any);
		}

	private:
		ConnectionReceiver mConnection;

		using VectorNode = std::vector<IAnimationNode*>;
		VectorNode mNodes;

		using MapAny = std::map<std::string, Ogre::Any, std::less<>>;
		MapAny mDatas;
	};

} // namespace animation

#endif // ANIMATION_GRAPH_H_
