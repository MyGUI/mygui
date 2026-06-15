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

	class GraphNodeSkeletonState : public BaseAnimationNode
	{
	public:
		GraphNodeSkeletonState(std::string_view _name) :
			BaseAnimationNode("GraphNodeSkeletonState.layout", "SkeletonState", _name)
		{
		}

	private:
		void initialise() override
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
			MyGUI::Gui::getInstance().eventFrameStart +=
				MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyFrameStart);
		}

		void shutdown() override
		{
			MyGUI::Gui::getInstance().eventFrameStart -=
				MyGUI::newDelegate(this, &GraphNodeSkeletonState::notifyFrameStart);
		}

		void deserialization(MyGUI::xml::ElementPtr _node) override
		{
			MyGUI::xml::ElementEnumerator prop = _node->getElementEnumerator();
			while (prop.next("Property"))
			{
				std::string_view key = prop->findAttribute("key");
				std::string_view value = prop->findAttribute("value");

				if (key == "StateName")
				{
					setStateName(MyGUI::UString(value));
				}
			}
		}

		void serialization(MyGUI::xml::ElementPtr _node) override
		{
			MyGUI::xml::ElementPtr prop = _node->createChild("Property");
			prop->addAttribute("key", "StateName");
			prop->addAttribute("value", mStateName);
		}

		void setStateName(const MyGUI::UString& _name)
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

		void baseInitialiseAnimationNode() override
		{
			Ogre::Any any = getAnimationNode()->getGraph()->getData("OwnerEntity");
			if (any.has_value())
			{
				Ogre::Entity* entity = Ogre::any_cast<Ogre::Entity*>(any);
				Ogre::AnimationStateSet* set = entity->getAllAnimationStates();
				for (const auto& state : set->getAnimationStates())
				{
					mComboStates->addItem(state.second->getAnimationName());
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
		wraps::BaseGraphConnection* mStartIn{nullptr};
		wraps::BaseGraphConnection* mStopIn{nullptr};
		wraps::BaseGraphConnection* mPositionIn{nullptr};
		wraps::BaseGraphConnection* mWeightIn{nullptr};
		MyGUI::ComboBox* mComboStates{nullptr};
		MyGUI::TextBox* mWeightValue{nullptr};
		MyGUI::TextBox* mPositionValue{nullptr};
		MyGUI::TextBox* mStartValue{nullptr};
		MyGUI::TextBox* mStopValue{nullptr};
	};

} // namespace demo

#endif // GRAPH_NODE_SKELETON_STATE_H_
