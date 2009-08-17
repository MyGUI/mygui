#ifndef __I_ANIMATION_NODE_H__
#define __I_ANIMATION_NODE_H__

#include <MyGUI.h>

namespace animation
{

	class IAnimationNode
	{
	public:
		IAnimationNode()
		{
		}

		IAnimationNode(const std::string& _name) :
			mName(_name)
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

		const std::string& getName()
		{
			return mName;
		}

	private:
		std::string mName;
	};

} // namespace animation

#endif // __I_ANIMATION_NODE_H__
