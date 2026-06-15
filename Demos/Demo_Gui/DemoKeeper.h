/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "EditorWindow.h"
#include "MainPanel.h"
#include "InformationWindow.h"
#include "ColourWindow.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void notifyEventAction(MainPanel::TypeEvents _action, size_t _index);
		void destroyWindows();
		void createWindows();
		void setupResources() override;
		void removeRenderBoxes();
		void createRenderBox(MyGUI::Canvas* _canvas);

		void createDefaultScene();
		void notifyFrameStart(float _time);

	private:
		EditorWindow* mEditorWindow{nullptr};
		MainPanel* mMainPanel{nullptr};
		InformationWindow* mInformationWindow{nullptr};
		ColourWindow* mColourWindow{nullptr};

#ifdef MYGUI_OGRE_PLATFORM
		Ogre::SceneNode* mNode{nullptr};
#endif // MYGUI_OGRE_PLATFORM
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
