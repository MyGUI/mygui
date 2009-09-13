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
#ifndef __MYGUI_OPENGL_TEXTURE_MANAGER_H__
#define __MYGUI_OPENGL_TEXTURE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_TextureManager.h"

namespace MyGUI
{

	class OpenGLTextureManager :
		public TextureManager
	{
		MYGUI_INSTANCE_HEADER(OpenGLTextureManager);

	public:
		void initialise();
		void shutdown();

		virtual ITexture* createTexture(const std::string& _name);
		virtual void destroyTexture(ITexture* _texture);
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_TEXTURE_MANAGER_H__
