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

		AnimationGraph(const std::string& _name) :
			IAnimationGraph(_name)
		{
		}

		~AnimationGraph() override
		{
		}

		void setEvent(const std::string& _name, float _value = 0) override
		{
			mConnection.forceEvent(_name, _value);
		}

		void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin) override
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		void removeConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin) override
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

		IAnimationNode* getNodeByName(const std::string& _name) override
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

		Ogre::Any getData(const std::string& _name) override
		{
			MapAny::iterator item = mDatas.find(_name);
			if (item != mDatas.end()) return item->second;
			return Ogre::Any();
		}

		void addData(const std::string& _name, Ogre::Any _any) override
		{
			mDatas[_name] = _any;
		}

	private:
		ConnectionReceiver mConnection;

		typedef std::vector<IAnimationNode*> VectorNode;
		VectorNode mNodes;

		typedef std::map<std::string, Ogre::Any> MapAny;
		MapAny mDatas;

	};

} // namespace animation

#endif // ANIMATION_GRAPH_H_
