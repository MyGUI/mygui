/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __ANIMATION_GRAPH_H__
#define __ANIMATION_GRAPH_H__

#include "IBase.h"
#include "IAnimationController.h"
#include "IAnimationState.h"
#include "IRenderableObject.h"
#include "AnimationFactory.h"
#include "IAnimationGraph.h"

namespace sim
{

	class AnimationGraph : public anim::IAnimationGraph
	{
	public:
		AnimationGraph(IBase * _owner, const std::string& _filename, const anim::VectorLink& _links)
		{
			mLinks = _links;
			// для убыстрения кастов
			_owner = _owner->queryType<IRenderableObject>();
			IRenderableObject * rend = _owner->queryType<IRenderableObject>();

			MyGUI::xml::Document doc;

			MYGUI_ASSERT(doc.open(MyGUI::helper::getResourcePath(_filename)), doc.getLastError());

			MyGUI::xml::ElementEnumerator item = doc.getRoot()->getElementEnumerator();
			while (item.next()) {

				if (item->getName() == "State") {
					anim::IAnimationState * state = anim::AnimationFactory::createState(this, _owner, item.current());
					mStates.push_back(state);
				}
				else if (item->getName() == "Controller") {
					anim::IAnimationController * controller = anim::AnimationFactory::createController(this, _owner, item.current());
					mControllers.push_back(controller);
				}
				else if (item->getName() == "Link") {
					LinkNodes(
						item->findAttribute("id_exit"),
						item->findAttribute("name_exit"),
						item->findAttribute("id_enter"),
						item->findAttribute("name_enter"),
						MyGUI::utility::parseBool(item->findAttribute("event")));
				}

			};
		}

		void update(float _time)
		{
			for (size_t index=0; index<mControllers.size(); ++index) {
				mControllers[index]->update(_time);
			}
		}

		void addExternalLink(anim::IAnimationLink * _link)
		{
			mLinks.push_back(_link);
		}

		virtual float getAnimationLength(const std::string& _name)
		{
			for (size_t pos=0; pos<mStates.size(); ++pos) {
				if (mStates[pos]->getName() == _name) return mStates[pos]->getLength();
			}
			MYGUI_EXCEPT("state '" << _name << "' not found");
			return 0;
		}

	private:
		void LinkNodes(const std::string& _nodeExit, const std::string& _eventExit, const std::string& _nodeEnter, const std::string& _eventEnter, bool _event)
		{
			anim::IAnimationLink * exit = 0;
			for (size_t pos=0; pos<mStates.size(); ++pos) {
				if (mStates[pos]->getName() == _nodeExit) {
					exit = mStates[pos];
					break;
				}
			}
			for (size_t pos=0; pos<mControllers.size(); ++pos) {
				if (mControllers[pos]->getName() == _nodeExit) {
					exit = mControllers[pos];
					break;
				}
			}
			for (size_t pos=0; pos<mLinks.size(); ++pos) {
				if (mLinks[pos]->getName() == _nodeExit) {
					exit = mLinks[pos];
					break;
				}
			}
			MYGUI_ASSERT(exit, "node '" << _nodeExit << "' not found");

			anim::IAnimationLink * enter = 0;
			for (size_t pos=0; pos<mStates.size(); ++pos) {
				if (mStates[pos]->getName() == _nodeEnter) {
					enter = mStates[pos];
					break;
				}
			}
			for (size_t pos=0; pos<mControllers.size(); ++pos) {
				if (mControllers[pos]->getName() == _nodeEnter) {
					enter = mControllers[pos];
					break;
				}
			}
			for (size_t pos=0; pos<mLinks.size(); ++pos) {
				if (mLinks[pos]->getName() == _nodeEnter) {
					enter = mLinks[pos];
					break;
				}
			}
			MYGUI_ASSERT(enter, "node '" << _nodeEnter << "' not found");

			if (_event) {
				enter->setLinkEvent(exit->getLinkEvent(_eventExit), _eventEnter);
			}
			else {
				enter->setLinkValue(exit->getLinkValue(_eventExit), _eventEnter);
			}
		}

	private:
		anim::VectorController mControllers;
		anim::VectorState mStates;
		anim::VectorLink mLinks;
	};

	typedef std::vector<AnimationGraph*> VectorGraph;

} // namespace sim

#endif // __ANIMATION_GRAPH_H__
