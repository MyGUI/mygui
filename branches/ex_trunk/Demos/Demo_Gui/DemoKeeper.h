/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "EditorWindow.h"
#include "MainPanel.h"
#include "InformationWindow.h"
#include "ColourWindow.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
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

	private:
		EditorWindow * mEditorWindow;
		MainPanel * mMainPanel;
		InformationWindow * mInformationWindow;
		ColourWindow * mColourWindow;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
