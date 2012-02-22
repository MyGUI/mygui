/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __LOOP_CONTROLLER_H__
#define __LOOP_CONTROLLER_H__

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class LoopController :
		public IAnimationNode
	{
	public:
		LoopController() :
			IAnimationNode(),
			mLength(0),
			mCurrentTime(0),
			mIsAnimationRun(false),
			mState(0)
		{
		}

		LoopController(const std::string& _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph),
			mLength(0),
			mCurrentTime(0),
			mIsAnimationRun(false),
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
			else if (_name == "Weight") mConnection.forceEvent("Weight", _value);
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
				mConnection.forceEvent("Position", mCurrentTime);
			}
		}

		virtual void setProperty(const std::string& _key, const std::string& _value)
		{
			if (_key == "LengthByState")
			{
				mState = getGraph()->getNodeByName(_value);
			}
			else if (_key == "Length")
			{
				mLength = MyGUI::utility::parseValue<float>(_value);
			}
		}

	private:
		void start()
		{
			mCurrentTime = 0;
			mIsAnimationRun = true;
			mConnection.forceEvent("Start");
		}

		void stop()
		{
			mIsAnimationRun = false;
			mConnection.forceEvent("Stop");
		}

	private:
		float mLength;
		float mCurrentTime;
		bool mIsAnimationRun;
		IAnimationNode* mState;

		ConnectionReceiver mConnection;

	};

} // namespace animation

#endif // __LOOP_CONTROLLER_H__
