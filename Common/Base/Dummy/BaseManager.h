#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_DummyPlatform.h>

namespace base
{

	class BaseManager :
		public SdlBaseManager
	{
	public:
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

	private:
		MyGUI::DummyPlatform* mPlatform = nullptr;
	};

}
