/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __RANDOM_CONTROLLER_H__
#define __RANDOM_CONTROLLER_H__

#include "IBase.h"
#include "IAnimationController.h"
#include "IAnimationState.h"
#include "AnimationFactory.h"

namespace anim
{

	class RandomController : public IAnimationController
	{
	public:
		typedef std::map<std::string, DelegateLinkEvent> MapEvent;

	public:
		RandomController(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::xmlNodePtr _node)
		{
			mName = _node->findAttribute("id");
		}

		virtual void update(float _time) { }
		virtual const std::string& getName() { return mName; }

		virtual DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			MapEvent::iterator iter = mEvents.find(_name);
			if (iter == mEvents.end()) {
				iter = mEvents.insert(std::make_pair(_name, DelegateLinkEvent())).first;
			}
			return &iter->second;
		}
		virtual DelegateLinkValue * getLinkValue(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual void setLinkEvent(DelegateLinkEvent * _delegate, const std::string& _name)
		{
			if (_name == "start") {
				*_delegate += MyGUI::newDelegate(this, &RandomController::eventStart);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkValue(DelegateLinkValue * _delegate, const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}

		virtual ~RandomController() { }

	private:
		int random(int _max)
		{
			int result = rand() % _max;
			return result < 0 ? 0 : result;
		}

		void eventStart(float _time)
		{
			int num = random(mEvents.size());
			MapEvent::iterator iter = mEvents.begin();
			while (num != 0) {
				--num;
				++iter;
			}
			iter->second(_time);
		}

	private:
		MapEvent mEvents;
		std::string mName;

	};

} // namespace anim

#endif // __RANDOM_CONTROLLER_H__
