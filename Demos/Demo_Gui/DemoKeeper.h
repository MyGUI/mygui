/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "EditorWindow.h"
#include "MainPanel.h"
#include "InformationWindow.h"
#include "ItemBoxWindow.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyEventAction(MainPanel::TypeEvents _action, size_t _index);

	private:
		EditorWindow mEditorWindow;
		MainPanel mMainPanel;
		InformationWindow mInformationWindow;
		ItemBoxWindow mItemBoxWindow;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
