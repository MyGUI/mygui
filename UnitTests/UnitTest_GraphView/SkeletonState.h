/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef __SKELETON_STATE_H__
#define __SKELETON_STATE_H__

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
			mState(0)
		{
		}

		SkeletonState(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph),
			mState(0)
		{
		}

		virtual ~SkeletonState()
		{
			if (mState != 0) mState->setEnabled(false);
		}

		virtual void setEvent(const std::string& _name, float _value = 0)
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

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnections.push_back(PairOut(_eventout, PairIn(_node, _eventin)));
		}

		virtual void setProperty(const std::string& _key, const std::string& _value)
		{
			if (_key == "StateName")
			{
				if (mState != 0) mState->setEnabled(false);
				mState = 0;
				mStateName = _value;
			}
		}

		virtual float getLength()
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
			if (mState == 0) return 0;
			return mState->getWeight();
		}

		float getPosition()
		{
			if (mState == 0) return 0;
			return mState->getTimePosition();
		}

		bool isEnabled()
		{
			if (mState == 0) return false;
			return mState->getEnabled();
		}

	private:
		void updateState()
		{
			Ogre::Any any = getGraph()->getData("OwnerEntity");
			if (!any.isEmpty())
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

#endif // __SKELETON_STATE_H__
