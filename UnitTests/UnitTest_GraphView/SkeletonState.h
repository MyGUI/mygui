/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef SKELETON_STATE_H_
#define SKELETON_STATE_H_

#include <Ogre.h>
#include "IAnimationNode.h"
#include "IAnimationGraph.h"

namespace animation
{

	class SkeletonState :
		public IAnimationNode
	{
	public:
		SkeletonState() :
			mState(nullptr)
		{
		}

		SkeletonState(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph),
			mState(nullptr)
		{
		}

		~SkeletonState() override
		{
			if (mState != nullptr) mState->setEnabled(false);
		}

		void setEvent(const std::string& _name, float _value = 0) override
		{
			if (!mState)
			{
				updateState();
				if (!mState) return;
			}

			if (_name == "Start") mState->setEnabled(true);
			else if (_name == "Stop") mState->setEnabled(false);
			else if (_name == "Position") mState->setTimePosition(_value);
			else if (_name == "Weight") mState->setWeight(_value);
		}

		void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin) override
		{
			mConnections.push_back(PairOut(_eventout, PairIn(_node, _eventin)));
		}

		void setProperty(const std::string& _key, const std::string& _value) override
		{
			if (_key == "StateName")
			{
				if (mState != nullptr) mState->setEnabled(false);
				mState = nullptr;
				mStateName = _value;
			}
		}

		float getLength() override
		{
			if (!mState)
			{
				updateState();
				if (!mState) return 0;
			}
			return mState->getLength();
		}

		float getWeight()
		{
			if (mState == nullptr) return 0;
			return mState->getWeight();
		}

		float getPosition()
		{
			if (mState == nullptr) return 0;
			return mState->getTimePosition();
		}

		bool isEnabled()
		{
			if (mState == nullptr) return false;
			return mState->getEnabled();
		}

	private:
		void updateState()
		{
			Ogre::Any any = getGraph()->getData("OwnerEntity");
			if (any.has_value())
			{
				Ogre::Entity* entity = Ogre::any_cast<Ogre::Entity*>(any);
				entity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
				mState = entity->getAnimationState(mStateName);
			}
		}

	private:
		Ogre::AnimationState* mState;
		typedef std::pair<IAnimationNode*, std::string> PairIn;
		typedef std::pair<std::string, PairIn> PairOut;
		typedef std::vector<PairOut> VectorPairOut;
		VectorPairOut mConnections;

		std::string mStateName;
	};

} // namespace animation

#endif // SKELETON_STATE_H_
