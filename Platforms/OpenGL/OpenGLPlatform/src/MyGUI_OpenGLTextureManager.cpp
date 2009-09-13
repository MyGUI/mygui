/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_OpenGLTextureManager.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_OpenGLDiagnostic.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OpenGLTextureManager);

	void OpenGLTextureManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLTextureManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	ITexture* OpenGLTextureManager::createTexture(const std::string& _name)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return nullptr;
	}

	void OpenGLTextureManager::destroyTexture(ITexture* _texture)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

} // namespace MyGUI
