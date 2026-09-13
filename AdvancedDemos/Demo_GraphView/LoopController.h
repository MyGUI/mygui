/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef LOOP_CONTROLLER_H_
#define LOOP_CONTROLLER_H_

#include "IAnimationNode.h"
#include "IAnimationGraph.h"
#include "ConnectionReceiver.h"

namespace animation
{

	class LoopController : public IAnimationNode
	{
	public:
		LoopController() = default;

		LoopController(std::string_view _name, IAnimationGraph* _graph) :
			IAnimationNode(_name, _graph)
		{
		}

		void setEvent(std::string_view _name, float _value = 0) override
		{
			if (_name == "Start")
				start();
			else if (_name == "Stop")
				stop();
			else if (_name == "Weight")
				mConnection.forceEvent("Weight", _value);
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
			if (mIsAnimationRun)
			{
				if (mLength != 0)
				{
					mCurrentTime += _value;
					while (mCurrentTime > mLength)
						mCurrentTime -= mLength;
				}
				else
				{
					if (mState)
					{
						mLength = mState->getLength();
						if (mLength != 0)
						{
							mCurrentTime += _value;
							while (mCurrentTime > mLength)
								mCurrentTime -= mLength;
						}
					}
				}
				mConnection.forceEvent("Position", mCurrentTime);
			}
		}

		void setProperty(std::string_view _key, std::string_view _value) override
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
		float mLength{0};
		float mCurrentTime{0};
		bool mIsAnimationRun{false};
		IAnimationNode* mState{nullptr};

		ConnectionReceiver mConnection;
	};

} // namespace animation

#endif // LOOP_CONTROLLER_H_
