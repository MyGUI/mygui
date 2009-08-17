/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __LOOP_CONTROLLER_H__
#define __LOOP_CONTROLLER_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"

namespace animation
{

	class LoopController : public IAnimationNode
	{
	public:
		LoopController() :
			IAnimationNode(),
			mLength(0),
			mCurrentTime(0),
			mIsAnimationRun(false),
			mHolder(0),
			mState(0)
		{
		}

		LoopController(const std::string& _name, IAnimationGraph* _holder) :
			IAnimationNode(_name),
			mLength(0),
			mCurrentTime(0),
			mIsAnimationRun(false),
			mHolder(_holder),
			mState(0)
		{
		}

		virtual ~LoopController()
		{
		}

		virtual void setEvent(const std::string& _name, float _value = 0)
		{
			if (_name == "Start") start();
			else if (_name == "Stop") stop();
			else if (_name == "Weight") forceEvent("Weight", _value);
		}

		virtual void addConnection(const std::string& _eventout, IAnimationNode* _node, const std::string& _eventin)
		{
			mConnections.push_back(PairOut(_eventout, PairIn(_node, _eventin)));
		}

		virtual void addTime(float _value)
		{
			if (mIsAnimationRun)
			{
				if (mLength != 0)
				{
					mCurrentTime += _value;
					while (mCurrentTime > mLength) mCurrentTime -= mLength;
				}
				else
				{
					if (mState)
					{
						mLength = mState->getLength();
						if (mLength != 0)
						{
							mCurrentTime += _value;
							while (mCurrentTime > mLength) mCurrentTime -= mLength;
						}
					}
				}
				forceEvent("Position", mCurrentTime);
			}
		}

		virtual void setProperty(const std::string& _key, const std::string& _value)
		{
			if (_key == "LengthByState")
			{
				mState = mHolder->getNodeByName(_value);
			}
			else if (_key == "Length")
			{
				mLength = MyGUI::utility::parseValue<float>(_value);
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

		void start()
		{
			mCurrentTime = 0;
			mIsAnimationRun = true;
			forceEvent("Start");
		}

		void stop()
		{
			mIsAnimationRun = false;
			forceEvent("Stop");
		}

	private:
		float mLength;
		float mCurrentTime;
		bool mIsAnimationRun;
		IAnimationGraph* mHolder;
		IAnimationNode* mState;

		typedef std::pair<IAnimationNode*, std::string> PairIn;
		typedef std::pair<std::string, PairIn> PairOut;
		typedef std::vector<PairOut> VectorPairOut;
		VectorPairOut mConnections;

	};

} // namespace animation

#endif // __LOOP_CONTROLLER_H__
