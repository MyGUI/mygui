/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_NODE_SKELETON_STATE_H_
#define GRAPH_NODE_SKELETON_STATE_H_

#include <MyGUI.h>
#include "BaseAnimationNode.h"
#include "SkeletonState.h"

namespace demo
{

	class GraphNodeSkeletonState :
		public BaseAnimationNode
	{
	public:
		GraphNodeSkeletonState(const std::string& _name) :
			BaseAnimationNode("GraphNodeSkeletonState.layout", "SkeletonState", _name),
			mStartIn(nullptr),
			mStopIn(nullptr),
			mPositionIn(nullptr),
			mWeightIn(nullptr),
			mComboStates(nullptr),
			mWeightValue(nullptr),
			mPositionValue(nullptr),
			mStartValue(nullptr),
			mStopValue(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());
			assignBase(mStartIn, "StartIn");
			assignBase(mStopIn, "StopIn");
			assignBase(mPositionIn, "PositionIn");
			assignBase(mWeightIn, "WeightIn");
			assignWidget(mComboStates, "ComboStates");
			assignWidget(mWeightValue, "WeightValue");
			assignWidget(mPositionValue, "PositionValue");
			assignWidget(mStartValue, "StartValue");
			assignWidget(mStopValue, "StopValue");

			mComboStates->eventComboAccept += MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyComboAccept);
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyFrameStart);
		}

		virtual void shutdown()
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyFrameStart);
		}

		virtual void deserialization(MyGUI::xml::ElementPtr _node)
		{
			MyGUI::xml::ElementEnumerator prop = _node->getElementEnumerator();
			while (prop.next("Property"))
			{
				const std::string& key = prop->findAttribute("key");
				const std::string& value = prop->findAttribute("value");

				if (key == "StateName")
				{
					setStateName(value);
				}
			}
		}

		virtual void serialization(MyGUI::xml::ElementPtr _node)
		{
			MyGUI::xml::ElementPtr prop = _node->createChild("Property");
			prop->addAttribute("key", "StateName");
			prop->addAttribute("value", mStateName);
		}

		void setStateName(const std::string& _name)
		{
			size_t index = mComboStates->findItemIndexWith(_name);
			if (index != MyGUI::ITEM_NONE)
			{
				mComboStates->setIndexSelected(index);
				notifyComboAccept(mComboStates, index);
			}
		}

		void notifyFrameStart(float _time)
		{
			animation::SkeletonState* state = dynamic_cast<animation::SkeletonState*>(getAnimationNode());
			if (state)
			{
				{
					float value = state->getWeight();
					std::ostringstream stream;
					stream.setf(std::ios::fixed, std::ios::floatfield);
					stream.precision(1);
					stream << value;
					mWeightValue->setCaption(stream.str());
				}
				{
					float value = state->getPosition();
					std::ostringstream stream;
					stream.setf(std::ios::fixed, std::ios::floatfield);
					stream.precision(1);
					stream << value;
					mPositionValue->setCaption(stream.str());
				}

				const MyGUI::Colour colour(1, 1, 1);

				if (state->isEnabled())
				{
					mStartValue->setTextColour(colour);
					mStopValue->setTextColour(MyGUI::Colour::Black);
				}
				else
				{
					mStartValue->setTextColour(MyGUI::Colour::Black);
					mStopValue->setTextColour(colour);
				}
			}
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
				mComboStates->setIndexSelected(0);
				notifyComboAccept(mComboStates, 0);
			}
		}

		void notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
		{
			if (_index != MyGUI::ITEM_NONE)
			{
				mStateName = _sender->getItemNameAt(_index);
				getAnimationNode()->setProperty("StateName", mStateName);
				eventInvalidateNode(this);
			}
		}

	private:
		std::string mStateName;
		wraps::BaseGraphConnection* mStartIn;
		wraps::BaseGraphConnection* mStopIn;
		wraps::BaseGraphConnection* mPositionIn;
		wraps::BaseGraphConnection* mWeightIn;
		MyGUI::ComboBox* mComboStates;
		MyGUI::TextBox* mWeightValue;
		MyGUI::TextBox* mPositionValue;
		MyGUI::TextBox* mStartValue;
		MyGUI::TextBox* mStopValue;

	};

} // namespace demo

#endif // GRAPH_NODE_SKELETON_STATE_H_
