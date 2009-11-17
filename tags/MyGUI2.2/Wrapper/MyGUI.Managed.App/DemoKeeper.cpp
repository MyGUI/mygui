/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#include "DemoKeeper.h"
#include <vcclr.h>

namespace MyGUI
{
	namespace Managed
	{

		demo::DemoKeeper * instance = nullptr;

		public value struct ExampleApplication
		{

			delegate void HandleFrameStart(float _time);
			static HandleFrameStart^ mFrameStartDelegate = nullptr;

			static void AddFrameDelegate(HandleFrameStart^ _delegate)
			{
				mFrameStartDelegate  = _delegate;
			}

			static void Initialise()
			{
				assert(!instance);
				instance = new demo::DemoKeeper();
				instance->create();
			}

			static void Run()
			{
				assert(instance);
				instance->run();

				mFrameStartDelegate  = nullptr;
				instance->destroy();
				delete instance;
				instance = 0;
			}

			static void Shutdown()
			{
				assert(instance);
				instance->exit();
			}

			static void DebugOut(System::String^ _out)
			{
				assert(instance);
				cli::pin_ptr<const wchar_t> str = PtrToStringChars(_out);
				Ogre::UTFString utf(str);
				MyGUI::MYGUI_OUT(utf.asUTF8_c_str());
			}

		};

	}
}

namespace demo
{

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Wrapper");
	}

	void DemoKeeper::destroyScene()
	{
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		bool ok = BaseManager::frameStarted(evt);
		if (ok && MyGUI::Managed::ExampleApplication::mFrameStartDelegate  != nullptr) MyGUI::Managed::ExampleApplication::mFrameStartDelegate (evt.timeSinceLastFrame);
		return ok;
	}

	bool DemoKeeper::frameEnded(const Ogre::FrameEvent& evt)
	{
		return BaseManager::frameEnded(evt);
	}

} // namespace demo
