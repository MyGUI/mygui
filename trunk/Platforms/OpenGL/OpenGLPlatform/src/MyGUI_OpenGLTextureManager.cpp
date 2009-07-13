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

#include <d3dx9.h>

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OpenGLTextureManager);

	void OpenGLTextureManager::initialise(IDirect3DDevice9 *_device)
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mpD3DDevice = _device;

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLTextureManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mpD3DDevice = nullptr;

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	ITexture* OpenGLTextureManager::createTexture(const std::string& _name)
	{
		MapResource::const_iterator item = mResources.find(_name);
		MYGUI_PLATFORM_ASSERT(item==mResources.end(), "Resource '" << _name << "' already exist");

		OpenGLTexture* texture = new OpenGLTexture(mpD3DDevice, _name);
		mResources[_name] = texture;

		return texture;
	}

	void OpenGLTextureManager::destroyTexture(ITexture* _texture)
	{
		remove(_texture->getName());
	}

} // namespace MyGUI
