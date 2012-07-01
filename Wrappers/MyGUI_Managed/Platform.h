/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MyGUI_DummyPlatform.h"

//#include "Config.h"
#include "Utility.h"

namespace MyGUI
{
	namespace Managed
	{
		public ref class Platform
		{
		public:
			static void CreatePlatform(System::String ^ _logFileName)
			{
				mDummyPlatform = new MyGUI::DummyPlatform();
				mDummyPlatform->initialise(string_utility::managed_to_utf8(_logFileName));
			}

			static void DestroyPlatform()
			{
				mDummyPlatform->shutdown();
				delete mDummyPlatform;
				mDummyPlatform = nullptr;
			}

			static void CreateGui(System::String ^ _coreFileName)
			{
				mGui = new MyGUI::Gui();
				mGui->initialise(string_utility::managed_to_utf8(_coreFileName));
			}

			static void DestroyGui()
			{
				mGui->shutdown();
				delete mGui;
				mGui = nullptr;
			}

		private:
			static MyGUI::Gui* mGui = nullptr;
			static MyGUI::DummyPlatform* mDummyPlatform = nullptr;
		};
	}
}