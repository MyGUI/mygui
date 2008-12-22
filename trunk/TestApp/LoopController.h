/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __LOOP_CONTROLLER_H__
#define __LOOP_CONTROLLER_H__

#include "IBase.h"
#include "IAnimationController.h"
#include "IAnimationState.h"
#include "AnimationFactory.h"

namespace anim
{

	class LoopController : public IAnimationController
	{
	public:
		LoopController(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node, const VectorState& _states) :
			m_startTime(0),
			m_weight(1),
			m_length(0),
			mCount(-1)
		{
			mName = _node->findAttribute("id");
			m_length = AnimationFactory::getTime(_node->findAttribute("time"), _states);
			std::string count;
			if (_node->findAttribute("count", count)) {
				mCount = MyGUI::utility::parseSizeT(count);
			}
		}

		virtual void update(float _time)
		{
			if (m_startTime == 0) return;
			size_t count = 1;
			while (m_startTime + m_length < _time) {
				if (mCount != -1) {
					if (count < mCount) {
						count++;
					}
					else {
						eventStop(_time);
						return;
					}
				}

				_time -= m_length;
			}
			float pos = (_time - m_startTime) / m_length;
			eventExitValue(_time, pos);

		}
		virtual const std::string& getName() { return mName; }

		virtual DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			if (_name == "exit_start") {
				return &eventExitStart;
			}
			else if (_name == "exit_stop") {
				return &eventExitStop;
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual DelegateLinkValue * getLinkValue(const std::string& _name)
		{
			if (_name == "exit_value") {
				return &eventExitValue;
			}
			else if (_name == "exit_weight") {
				return &eventExitWeight;
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkEvent(DelegateLinkEvent * _delegate, const std::string& _name)
		{
			if (_name == "start") {
				*_delegate += MyGUI::newDelegate(this, &LoopController::eventStart);
			}
			else if (_name == "stop") {
				*_delegate += MyGUI::newDelegate(this, &LoopController::eventStop);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkValue(DelegateLinkValue * _delegate, const std::string& _name)
		{
			if (_name == "weight") {
				*_delegate += MyGUI::newDelegate(this, &LoopController::setWeight);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}

		virtual ~LoopController() { }

	private:
		void setWeight(float _time, float _value) { m_weight = _value; }
		void eventStart(float _time)
		{
			m_startTime = _time;
			mLastLoopTime = _time;
			eventExitWeight(_time, m_weight);
			eventExitStart(_time);
		}
		void eventStop(float _time)
		{
			m_startTime = 0;
			eventExitWeight(_time, 0);
			eventExitStop(_time);
		}

	private:
		DelegateLinkValue eventExitValue;
		DelegateLinkValue eventExitWeight;
		DelegateLinkEvent eventExitStart;
		DelegateLinkEvent eventExitStop;

	private:
		float m_length;
		float m_startTime;
		float m_weight;
		std::string mName;
		size_t mCount;
		float mLastLoopTime;
	};

} // namespace anim

#endif // __LOOP_CONTROLLER_H__
