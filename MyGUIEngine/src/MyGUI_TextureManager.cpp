/*!
	@file
	@author		Albert Semenov
	@date		04/2009
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
#include "MyGUI_TextureManager.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_Bitwise.h"

namespace MyGUI
{

	const std::string INSTANCE_TYPE_NAME("TextureManager");

	TextureManager* TextureManager::msInstance = 0;

	TextureManager::TextureManager() :
		mIsInitialise(false)
	{
		MYGUI_ASSERT(0 == msInstance, "instance " << INSTANCE_TYPE_NAME << " is exsist");
		msInstance = this;
	}

	TextureManager::~TextureManager()
	{
		msInstance = 0;
	}

	TextureManager* TextureManager::getInstancePtr()
	{
		return msInstance;
	}

	TextureManager& TextureManager::getInstance()
	{
		MYGUI_ASSERT(0 != msInstance, "instance " << INSTANCE_TYPE_NAME << " was not created");
		return (*msInstance);
	}

	const IntSize& TextureManager::getTextureSize(const std::string& _texture)
	{
		// предыдущ€ текстура
		static std::string old_texture;
		static IntSize old_size;

		if (old_texture == _texture) return old_size;
		old_texture = _texture;
		old_size.clear();

		if (_texture.empty()) return old_size;

		if (nullptr == findByName(_texture))
		{
			DataManager& resourcer = DataManager::getInstance();
			if (!resourcer.isDataExist(_texture))
			{
				MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
				return old_size;
			}
			else
			{
				ITexture* texture = createTexture(_texture);
				texture->loadFromFile(_texture);
			}
		}

		ITexture* texture = findByName(_texture);
		if (texture == nullptr)
		{
			MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
			return old_size;
		}

		old_size.set(texture->getWidth(), texture->getHeight());

#if MYGUI_DEBUG_MODE == 1
		if (!Bitwise::isPO2(old_size.width) || !Bitwise::isPO2(old_size.height))
		{
			MYGUI_LOG(Warning, "Texture '" + _texture + "' have non power of two size");
		}
#endif

		return old_size;
	}

} // namespace MyGUI
