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

	class AnimationGraph :
		public IAnimationGraph
	{
	public:
		AnimationGraph() :
			IAnimationGraph()
		{
		}

		AnimationGraph(std::string_view _name) :
			IAnimationGraph(_name)
		{
		}

		~AnimationGraph() override
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
			for (VectorNode::iterator item = mNodes.begin(); item != mNodes.end(); ++item)
			{
				(*item)->addTime(_value);
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
			if (_name == getName()) return this;

			for (VectorNode::iterator item = mNodes.begin(); item != mNodes.end(); ++item)
			{
				if ((*item)->getName() == _name)
				{
					return (*item);
				}
			}
			return nullptr;
		}

		Ogre::Any getData(std::string_view _name) override
		{
			MapAny::iterator item = mDatas.find(_name);
			if (item != mDatas.end()) return item->second;
			return Ogre::Any();
		}

		void addData(std::string_view _name, Ogre::Any _any) override
		{
			auto it = mDatas.find(_name);
			if (it == mDatas.end())
				mDatas.emplace(_name, _any);
			else
				it->second = _any;
		}

	private:
		ConnectionReceiver mConnection;

		typedef std::vector<IAnimationNode*> VectorNode;
		VectorNode mNodes;

		typedef std::map<std::string, Ogre::Any, std::less<>> MapAny;
		MapAny mDatas;

	};

} // namespace animation

#endif // ANIMATION_GRAPH_H_
