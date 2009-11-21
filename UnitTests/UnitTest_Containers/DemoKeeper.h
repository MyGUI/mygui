/*!
	@file
	@author		Ptakhin Alexander
	@date		02/2009
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "MyGUI_FlowContainer.h"

namespace demo
{
	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper() : base::BaseManager() { }
		virtual void createScene();
		virtual void destroyScene();
		//virtual bool keyPressed(const OIS::KeyEvent &arg);

		//virtual bool frameStarted(const Ogre::FrameEvent & evt);

		//virtual bool mouseMoved(const OIS::MouseEvent &arg);

		void clear();
		void codeTest();
		void xmlTest();

		//virtual bool keyPressed(const OIS::KeyEvent &arg);

		void buttonClicked(MyGUI::Widget* _sender);
	private:
		virtual void setupResources();

	private:
		MyGUI::FlowContainerPtr mFlow;

		std::vector< MyGUI::ButtonPtr > mButtons;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
