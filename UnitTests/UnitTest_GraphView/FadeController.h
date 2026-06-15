/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef FADE_CONTROLLER_H_
#define FADE_CONTROLLER_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class FadeController : public IAnimationNode
	{
	public:
		FadeController() :
			IAnimationNode(),
			mIsAnimationRun(false),
			mWeight(0)
		{
		}

		FadeController(std::string_view _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph),
			mIsAnimationRun(false),
			mWeight(0)
		{
		}

		void setEvent(std::string_view _name, float _value = 0) override
		{
			if (_name == "Start")
			{
				mIsAnimationRun = true;
				if (mWeight == 0)
					mConnection.forceEvent("Start");
			}
			else if (_name == "Stop")
				mIsAnimationRun = false;
		}

		void addConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin) override
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		void removeConnection(std::string_view _eventout, IAnimationNode* _node, std::string_view _eventin) override
		{
			mConnection.removeConnection(_eventout, _node, _eventin);
		}

		void addTime(float _value) override
		{
			const float fade_time = 0.3f;
			if (mIsAnimationRun)
			{
				if (mWeight != 1)
				{
					mWeight += _value * (1 / fade_time);
					if (mWeight > 1)
						mWeight = 1;
					mConnection.forceEvent("Weight", mWeight);
				}
			}
			else
			{
				if (mWeight != 0)
				{
					mWeight -= _value * (1 / fade_time);
					if (mWeight < 0)
						mWeight = 0;
					mConnection.forceEvent("Weight", mWeight);

					if (mWeight == 0)
						mConnection.forceEvent("Stop");
				}
			}
		}

	private:
		ConnectionReceiver mConnection;

		bool mIsAnimationRun;
		float mWeight;
	};

} // namespace animation

#endif // FADE_CONTROLLER_H_
