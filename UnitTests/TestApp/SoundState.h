/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __SOUND_STATE_H__
#define __SOUND_STATE_H__

#include <Ogre.h>
#include "IBase.h"
#include "IRenderableObject.h"
#include "IAnimationState.h"

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#include <mmsystem.h> // для звука
#pragma comment(lib,"WinMM.lib")
#endif

namespace anim
{

	class SoundState : public anim::IAnimationState
	{
	public:
		SoundState(IAnimationGraph * _parent, sim::IBase * _owner, MyGUI::xml::ElementPtr _node)
		{
			mName = _node->findAttribute("id");
			mSource = MyGUI::helper::getResourcePath(_node->findAttribute("source"));
		}

		virtual float getLength() { return 1; }
		virtual const std::string& getName() { return mName; }

		virtual anim::DelegateLinkEvent * getLinkEvent(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual anim::DelegateLinkValue * getLinkValue(const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}
		virtual void setLinkEvent(anim::DelegateLinkEvent * _delegate, const std::string& _name)
		{
			if (_name == "start") {
				*_delegate += MyGUI::newDelegate(this, &SoundState::eventStart);
			}
			else {
				MYGUI_EXCEPT("link '" << _name << "' not found");
			}
		}
		virtual void setLinkValue(anim::DelegateLinkValue * _delegate, const std::string& _name)
		{
			MYGUI_EXCEPT("link '" << _name << "' not found");
		}

		virtual ~SoundState() { }

	private:
		void eventStart(float _time)
		{
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
			//::sndPlaySound(mSource.c_str(), SND_ASYNC | SND_NODEFAULT);
			::mciSendString(("stop " + mSource).c_str(), NULL, 0, NULL);
			::mciSendString(("play " + mSource).c_str(), NULL, 0, NULL);
#endif
		}

		void eventStop(float _time)
		{
		}

	private:
		std::string mName;
		std::string mSource;
	};

} // namespace anim

#endif // __SOUND_STATE_H__
