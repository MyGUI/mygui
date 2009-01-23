/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __ANIMATEBLE_OBJECT_H__
#define __ANIMATEBLE_OBJECT_H__

#include "IBase.h"
#include "AnimationGraph.h"
#include "TimeContext.h"

namespace sim
{

	class AnimatebleObject : public IBase
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(AnimatebleObject, IBase);

		AnimatebleObject()
		{
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &AnimatebleObject::frameStart);
		}

		anim::IAnimationGraph * createAnimationGraph(const std::string& _filename)
		{
			AnimationGraph * graph = new AnimationGraph(this, _filename, mLinks);
			return graph;
		}

		void startGraph(anim::IAnimationGraph * _graph)
		{
			mGraphItems.push_back(static_cast<AnimationGraph*>(_graph));
		}

		void frameStart(float _time)
		{
			float time = context::TimeContext::getCurrentTime();

			for (size_t index=0; index<mGraphItems.size(); ++index) {
				mGraphItems[index]->update(time);
			}
		}

		void addExternalLink(anim::IAnimationLink * _link)
		{
			mLinks.push_back(_link);
		}

	private:
		VectorGraph mGraphItems;
		anim::VectorLink mLinks;
	};

} // namespace sim

#endif // __ANIMATEBLE_OBJECT_H__
