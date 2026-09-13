#ifndef I_ANIMATION_NODE_H_
#define I_ANIMATION_NODE_H_

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

		IAnimationNode(std::string_view _name, IAnimationGraph* _graph) :
			mName(_name),
			mGraph(_graph)
		{
		}

		virtual ~IAnimationNode() = default;

		virtual void setEvent(std::string_view _name, float _value = 0)
		{
		}

		virtual void addConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin)
		{
		}

		virtual void removeConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin)
		{
		}

		virtual void addTime(float _value)
		{
		}

		virtual void setProperty(std::string_view _key, std::string_view _value)
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

#endif // I_ANIMATION_NODE_H_
