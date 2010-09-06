#ifndef __I_ANIMATION_NODE_H__
#define __I_ANIMATION_NODE_H__

#include <MyGUI.h>

namespace animation
{

	class IAnimationGraph;

	class IAnimationNode
	{
	public:
		IAnimationNode() :
			mGraph(nullptr)
		{
		}

		IAnimationNode(const std::string& _name, IAnimationGraph* _graph) :
			mName(_name),
			mGraph(_graph)
		{
		}

		virtual ~IAnimationNode()
		{
		}

		virtual void setEvent(const std::string& _name, float _value = 0)
		{
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
		}

		virtual void removeConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
		}

		virtual void addTime(float _value)
		{
		}

		virtual void setProperty(const std::string& _key, const std::string& _value)
		{
		}

		virtual float getLength()
		{
			return 0;
		}

		const std::string& getName() const
		{
			return mName;
		}

		IAnimationGraph* getGraph() const
		{
			return mGraph;
		}

	private:
		std::string mName;
		IAnimationGraph* mGraph;

	};

} // namespace animation

#endif // __I_ANIMATION_NODE_H__
