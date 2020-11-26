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

		void createScene() override;
		void destroyScene() override;

	private:
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;
		void setupResources() override;
		void createGround();
		void createObject();

		void setupCamera();
		void updateCamera(int _x, int _y);

		void injectMouseMove(int _absx, int _absy, int _absz) override;
		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) override;

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
