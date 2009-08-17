/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __FADE_CONTROLLER_H__
#define __FADE_CONTROLLER_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"

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

		FadeController(const std::string& _name, IAnimationGraph* _holder) :
			IAnimationNode(_name),
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
					forceEvent("Start");
			}
			else if (_name == "Stop") mIsAnimationRun = false;
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnections.push_back(PairOut(_eventout, PairIn(_node, _eventin)));
		}

		virtual void addTime(float _value)
		{
			const float fade_time = 0.3;
			if (mIsAnimationRun)
			{
				if (mWeight != 1)
				{
					mWeight += _value * (1 / fade_time);
					if (mWeight > 1) mWeight = 1;
					forceEvent("Weight", mWeight);
				}
			}
			else
			{
				if (mWeight != 0)
				{
					mWeight -= _value * (1 / fade_time);
					if (mWeight < 0) mWeight = 0;
					forceEvent("Weight", mWeight);
	 
					if (mWeight == 0)
						forceEvent("Stop");
				}
			}
		}

	private:
		void forceEvent(const std::string& _name, float _value = 0)
		{
			for (VectorPairOut::iterator item=mConnections.begin(); item!=mConnections.end(); ++item)
			{
				if (_name == item->first)
					item->second.first->setEvent(item->second.second, _value);
			}
		}

	private:
		typedef std::pair<IAnimationNode*, std::string> PairIn;
		typedef std::pair<std::string, PairIn> PairOut;
		typedef std::vector<PairOut> VectorPairOut;
		VectorPairOut mConnections;

		bool mIsAnimationRun;
		float mWeight;
	};

} // namespace animation

#endif // __FADE_CONTROLLER_H__
