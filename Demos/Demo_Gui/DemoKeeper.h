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

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyEventAction(MainPanel::TypeEvents _action, size_t _index);
		void destroyWindows();
		void createWindows();
		virtual void setupResources();
		void removeRenderBoxes();
		void createRenderBox(MyGUI::Canvas* _canvas);

		void createDefaultScene();
		void notifyFrameStart(float _time);

	private:
		EditorWindow* mEditorWindow;
		MainPanel* mMainPanel;
		InformationWindow* mInformationWindow;
		ColourWindow* mColourWindow;

#ifdef MYGUI_OGRE_PLATFORM
		Ogre::SceneNode* mNode;
#endif // MYGUI_OGRE_PLATFORM
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
