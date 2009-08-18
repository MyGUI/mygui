/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_NODE_SKELETON_STATE_H__
#define __GRAPH_NODE_SKELETON_STATE_H__

#include <MyGUI.h>
#include "BaseAnimationNode.h"

namespace demo
{

	class GraphNodeSkeletonState : public BaseAnimationNode
	{
	public:
		GraphNodeSkeletonState(const std::string& _name) :
			BaseAnimationNode("GraphNodeSkeletonState.layout"),
			mName(_name),
			mStartIn(nullptr),
			mStopIn(nullptr),
			mPositionIn(nullptr),
			mWeightIn(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->setCaption(mName);
			assignBase(mStartIn, "StartIn");
			assignBase(mStopIn, "StopIn");
			assignBase(mPositionIn, "PositionIn");
			assignBase(mWeightIn, "WeightIn");
			assignWidget(mComboStates, "ComboStates");

			mComboStates->eventComboAccept = MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyComboAccept);
		}

		virtual void shutdown()
		{
		}

		virtual void baseInitialiseAnimationNode()
		{
			Ogre::Any any = getAnimationNode()->getGraph()->getData("OwnerEntity");
			if (!any.isEmpty())
			{
				Ogre::Entity* entity = Ogre::any_cast<Ogre::Entity*>(any);
				Ogre::AnimationStateSet* set = entity->getAllAnimationStates();
				Ogre::AnimationStateIterator iter = set->getAnimationStateIterator();
				while (iter.hasMoreElements())
				{ 
					Ogre::AnimationState* state = iter.getNext(); 
					mComboStates->addItem(state->getAnimationName());
				}
			}

			if (mComboStates->getItemCount() > 0)
			{
				mComboStates->setItemSelectedAt(0);
				notifyComboAccept(mComboStates, 0);
			}
		}

		void notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index)
		{
			if (_index != -1)
				getAnimationNode()->setProperty("StateName", _sender->getItemNameAt(_index));
		}

	private:
		std::string mName;
		wraps::BaseGraphConnection * mStartIn;
		wraps::BaseGraphConnection * mStopIn;
		wraps::BaseGraphConnection * mPositionIn;
		wraps::BaseGraphConnection * mWeightIn;
		MyGUI::ComboBoxPtr mComboStates;

	};

} // namespace demo

#endif // __GRAPH_NODE_SKELETON_STATE_H__
