/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __FADE_CONTROLLER_H__
#define __FADE_CONTROLLER_H__

#include "IAnimationController.h"
#include "AnimationFactory.h"

namespace anim
{

	class FadeController : public IAnimationController
	{
	public:
		FadeController(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node, const VectorState& _states) :
			m_startTime(0),
			m_weight(0),
			m_down(true)
		{
			mName = _node->findAttribute("id");
			m_fadeTimeStart = AnimationFactory::getTime(_node->findAttribute("time_start"), _states);
			m_fadeTimeStop = AnimationFactory::getTime(_node->findAttribute("time_stop"), _states);
		}

		virtual void update(float _time)
		{
			if (m_startTime == 0) return;
			// спускаемся на улицу
			if (m_down) {
				// все, мы на улице
				if (m_startTimeDown + m_fadeTimeDown < _time) {
					m_startTime = 0;
					m_weight = 0;
					eventExitWeight(_time, m_weight);
					eventExitEventStop(_time);
					return;
				}
				// длигнные ступеньки
				else {
					m_weight = 1 - ((_time - m_startTimeDown) / m_fadeTimeDown);
				}
			}
			// поднимаемся по леснитце
			else if (m_startTime + m_fadeTimeStart > _time) {
				m_weight = (_time - m_startTime) / m_fadeTimeStart;
			}
			// идем по площадке
			else {
				m_weight = 1;
			}
			eventExitWeight(_time, m_weight);
		}
		virtual const std::string& getName() { return mName; }

		virtual DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			if (_name == "exit_start") {
				return &eventExitEventStart;
			}
			else if (_name == "exit_stop") {
				return &eventExitEventStop;
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual DelegateLinkValue * getLinkValue(const std::string& _name)
		{
			if (_name == "exit_weight") {
				return &eventExitWeight;
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkEvent(DelegateLinkEvent * _delegate, const std::string& _name)
		{
			if (_name == "start") {
				*_delegate += MyGUI::newDelegate(this, &FadeController::eventStart);
			}
			else if (_name == "stop") {
				*_delegate += MyGUI::newDelegate(this, &FadeController::eventStop);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkValue(DelegateLinkValue * _delegate, const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}

		virtual ~FadeController() { }

	private:
		void eventStart(float _time)
		{
			if (m_down) {
				m_down = false;
				m_startTime = _time;
				eventExitWeight(_time, m_weight);
				eventExitEventStart(_time);
			}
		}
		void eventStop(float _time)
		{
			if (!m_down) {
				m_down = true;
				m_fadeTimeDown = m_fadeTimeStop * m_weight;
				m_startTimeDown = _time;
			}
		}

	private:
		DelegateLinkValue eventExitWeight;
		DelegateLinkEvent eventExitEventStart;
		DelegateLinkEvent eventExitEventStop;

	private:
		float m_fadeTimeStart;
		float m_fadeTimeStop;
		float m_startTime;
		float m_weight;
		float m_fadeTimeDown;
		float m_startTimeDown;
		std::string mName;
		bool m_down;
	};

} // namespace anim

#endif // __FADE_CONTROLLER_H__
