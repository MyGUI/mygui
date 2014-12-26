/*!
	@file
	@author     George Evmenov
	@date       08/2009
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "CommandManager.h"
#include "KeyboardPanel.h"
#include "MonitorPanel.h"

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
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		void setupResources();
		void createGround();
		void createObject();

		void setupCamera();
		void updateCamera(int _x, int _y);

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);

	private:
		KeyboardPanel* mKeyboardPanel;
		MonitorPanel* mMonitorPanel;

		CommandManager* mCommandManager;

		bool mRightButtonPressed;
		int mSaveCursorX;
		int mSaveCursorY;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_
