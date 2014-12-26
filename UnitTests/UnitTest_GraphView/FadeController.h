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

	class FadeController :
		public IAnimationNode
	{
	public:
		FadeController() :
			IAnimationNode(),
			mIsAnimationRun(false),
			mWeight(0)
		{
		}

		FadeController(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph),
			mIsAnimationRun(false),
			mWeight(0)
		{
		}

		virtual ~FadeController()
		{
		}

		virtual void setEvent(const std::string& _name, float _value = 0)
		{
			if (_name == "Start")
			{
				mIsAnimationRun = true;
				if (mWeight == 0)
					mConnection.forceEvent("Start");
			}
			else if (_name == "Stop") mIsAnimationRun = false;
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnection.addConnection(_eventout, _node, _eventin);
		}

		virtual void removeConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnection.removeConnection(_eventout, _node, _eventin);
		}

		virtual void addTime(float _value)
		{
			const float fade_time = 0.3f;
			if (mIsAnimationRun)
			{
				if (mWeight != 1)
				{
					mWeight += _value * (1 / fade_time);
					if (mWeight > 1) mWeight = 1;
					mConnection.forceEvent("Weight", mWeight);
				}
			}
			else
			{
				if (mWeight != 0)
				{
					mWeight -= _value * (1 / fade_time);
					if (mWeight < 0) mWeight = 0;
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
