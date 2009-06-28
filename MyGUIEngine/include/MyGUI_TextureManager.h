/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#ifndef __MYGUI_TEXTURE_MANAGER_H__
#define __MYGUI_TEXTURE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_ResourceHolder.h"

namespace MyGUI
{

	class MYGUI_EXPORT TextureManager :
		public ResourceHolder<ITexture>
	{
	public:
		TextureManager();
		virtual ~TextureManager() = 0;

		static TextureManager& getInstance();
		static TextureManager* getInstancePtr();

		virtual ITexture* createTexture(const std::string& _name) = 0;
		virtual void destroyTexture(ITexture* _texture) = 0;

		/** Get texture size in pixels
			@param _texture file name
		*/
		const IntSize& getTextureSize(const std::string& _texture);

	private:
		static TextureManager* msInstance;
		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_MANAGER_H__
