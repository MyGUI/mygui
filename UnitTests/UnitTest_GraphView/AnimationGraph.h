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

		virtual ~AnimationGraph()
		{
		}

		virtual void setEvent(const std::string& _name, float _value = 0)
		{
			mConnection.forceEvent(_name, _value);
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		virtual void removeConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnection.removeConnection(_eventout, _node, _eventin);
		}

		virtual void addTime(float _value)
		{
			for (VectorNode::iterator item = mNodes.begin(); item != mNodes.end(); ++item)
			{
				(*item)->addTime(_value);
			}
		}

		virtual void addNode(IAnimationNode* _node)
		{
			mNodes.push_back(_node);
		}

		virtual void removeNode(IAnimationNode* _node)
		{
			VectorNode::iterator item = std::find(mNodes.begin(), mNodes.end(), _node);
			assert(item != mNodes.end());
			mNodes.erase(item);
		}

		IAnimationNode* getNodeByName(const std::string& _name)
		{
			if (_name == getName()) return this;

			for (VectorNode::iterator item = mNodes.begin(); item != mNodes.end(); ++item)
			{
				if ((*item)->getName() == _name)
				{
					return (*item);
				}
			}
			return 0;
		}

		virtual Ogre::Any getData(const std::string& _name)
		{
			MapAny::iterator item = mDatas.find(_name);
			if (item != mDatas.end()) return item->second;
			return Ogre::Any();
		}

		virtual void addData(const std::string& _name, Ogre::Any _any)
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
