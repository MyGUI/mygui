/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef BASE_ANIMATION_NODE_H_
#define BASE_ANIMATION_NODE_H_

#include <MyGUI.h>
#include "BaseGraphNode.h"
#include "IAnimationNode.h"

namespace demo
{

	class BaseAnimationNode : public wraps::BaseGraphNode
	{
	public:
		BaseAnimationNode(std::string_view _layout, std::string_view _type, std::string_view _name) :
			BaseGraphNode(_layout),

			mType(_type),
			mName(_name)
		{
		}

		void setAnimationNode(animation::IAnimationNode* _node)
		{
			mAnimationNode = _node;
			baseInitialiseAnimationNode();
		}

		animation::IAnimationNode* getAnimationNode()
		{
			return mAnimationNode;
		}

		virtual void addConnection(std::string_view _eventout, BaseAnimationNode* _node, std::string_view _eventin)
		{
		}

		virtual void removeConnection(std::string_view _eventout, BaseAnimationNode* _node, std::string_view _eventin)
		{
		}

		virtual void invalidateNode(BaseAnimationNode* _sender)
		{
		}

		virtual void serialization(MyGUI::xml::ElementPtr _node)
		{
		}

		virtual void deserialization(MyGUI::xml::ElementPtr _node)
		{
		}

		const std::string& getName() const
		{
			return mName;
		}
		const std::string& getType() const
		{
			return mType;
		}

		/** Event : Invalidate node.\n
			signature : void method(BaseAnimationNode* _sender)
			@param _sender
		*/
		MyGUI::delegates::Delegate<BaseAnimationNode*> eventInvalidateNode;

	protected:
		virtual void baseInitialiseAnimationNode()
		{
		}

	private:
		animation::IAnimationNode* mAnimationNode{nullptr};
		std::string mType;
		std::string mName;
	};

} // namespace demo

#endif // BASE_ANIMATION_NODE_H_
