/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __RANDOM_CONTROLLER_5_H__
#define __RANDOM_CONTROLLER_5_H__

#include "IBase.h"
#include "IAnimationController.h"
#include "IAnimationState.h"
#include "AnimationFactory.h"

namespace anim
{

	class RandomController5 : public IAnimationController
	{
	public:
		RandomController5(sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node, const VectorState& _states)
		{
			mName = _node->findAttribute("id");
		}

		virtual void update(float _time) { }
		virtual const std::string& getName() { return mName; }

		virtual DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			if (_name == "exit_start1") {
				return &eventExitEvent1;
			}
			else if (_name == "exit_start2") {
				return &eventExitEvent2;
			}
			else if (_name == "exit_start3") {
				return &eventExitEvent3;
			}
			else if (_name == "exit_start4") {
				return &eventExitEvent4;
			}
			else if (_name == "exit_start5") {
				return &eventExitEvent5;
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual DelegateLinkValue * getLinkValue(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual void setLinkEvent(DelegateLinkEvent * _delegate, const std::string& _name)
		{
			if (_name == "start") {
				*_delegate += MyGUI::newDelegate(this, &RandomController5::eventStart);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkValue(DelegateLinkValue * _delegate, const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}

		virtual ~RandomController5() { }

	private:
		int random(int _max)
		{
			int result = rand() % _max;
			return result < 0 ? 0 : result;
		}

		void eventStart(float _time)
		{
			int num = random(5);
			switch(num)
			{
				case 0: eventExitEvent1(_time); break;
				case 1: eventExitEvent2(_time); break;
				case 2: eventExitEvent3(_time); break;
				case 3: eventExitEvent4(_time); break;
				case 4: eventExitEvent5(_time); break;
			};
		}

	private:
		DelegateLinkEvent eventExitEvent1;
		DelegateLinkEvent eventExitEvent2;
		DelegateLinkEvent eventExitEvent3;
		DelegateLinkEvent eventExitEvent4;
		DelegateLinkEvent eventExitEvent5;

	private:
		std::string mName;

	};

} // namespace anim

#endif // __RANDOM_CONTROLLER_5_H__
