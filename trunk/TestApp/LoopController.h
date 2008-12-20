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
			m_length(0)
		{
			mName = _node->findAttribute("id");
			m_length = AnimationFactory::getTime(_node->findAttribute("time"), _states);
		}

		virtual void update(float _currentTime)
		{
			if (m_startTime == 0) return;
			while (m_startTime + m_length < _currentTime)  _currentTime -= m_length;
			float pos = (_currentTime - m_startTime) / m_length;
			eventExitValue(_currentTime, pos);
		}
		virtual const std::string& getName() { return mName; }

		virtual DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
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
			eventExitWeight(_time, m_weight);
		}
		void eventStop(float _time)
		{
			m_startTime = 0;
			eventExitWeight(_time, 0);
		}

	private:
		DelegateLinkValue eventExitValue;
		DelegateLinkValue eventExitWeight;

	private:
		float m_length;
		float m_startTime;
		float m_weight;
		std::string mName;
	};

} // namespace anim

#endif // __LOOP_CONTROLLER_H__
