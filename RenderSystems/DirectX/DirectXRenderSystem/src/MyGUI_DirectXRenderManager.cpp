/*!
@file
@author		Albert Semenov
@date		04/2008
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
#include "MyGUI_DirectXRenderManager.h"
#include "MyGUI_DirectXTexture.h"
#include "MyGUI_DirectXVertexBuffer.h"

namespace MyGUI
{
	void scanFolder(VectorString& _result, const std::string& _folder, bool _recursive, const std::string& _mask, bool _fullpath)
	{
		std::string folder = _folder;
		if (!folder.empty()) folder += "\\";
		std::vector<std::string> dir;


		WIN32_FIND_DATA FindData;
		HANDLE file = FindFirstFile((folder + _mask).c_str(), &FindData);
		if (file != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::string name = FindData.cFileName;
				if ((name == ".") || (name == "..")) continue;

				// если скрытый то игнорируем
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) continue;

				// если это дирректория, то запускаем в ней поиск
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dir.push_back(folder + name);
				}
				else
				{
					if (_fullpath)
						_result.push_back(folder + name);
					else
						_result.push_back(name);
				}

			} while (FindNextFile(file, &FindData));
		}
		FindClose(file);

		if (_recursive)
		{
			// теперь проходим подкаталоги
			for (std::vector<std::string>::iterator iter = dir.begin(); iter!=dir.end(); ++iter)
			{
				scanFolder(_result, *iter, _recursive, _mask, _fullpath);
			}
		}
	}

  MYGUI_INSTANCE_IMPLEMENT(DirectXRenderManager);

  void DirectXRenderManager::initialise(IDirect3DDevice9 *_device)
  {
    // new
    mpD3DDevice = _device;

    mViewSize.set(1024, 1024);
    mTexelOffset.set(0, 0);
    mMaximumDepth = 0;
    mVertexFormat = VertexColourType::ColourABGR;  

    mIsInitialise = true;
  }

  void DirectXRenderManager::shutdown()
  {
    mpD3DDevice = nullptr;
    mIsInitialise = false;
  }

  ITexture* DirectXRenderManager::createTexture(const std::string& _name, const std::string& _group)
  {
    MapTexture::const_iterator item = mTextures.find(_name);
    MYGUI_ASSERT(item==mTextures.end(), "Resource '" << _name << "' already exist");

    DirectXTexture* texture = new DirectXTexture(mpD3DDevice, _name, _group);
    mTextures[_name] = texture;

    return texture;
  }

  void DirectXRenderManager::destroyTexture(ITexture* _texture)
  {
    for (MapTexture::iterator item=mTextures.begin(); item!=mTextures.end(); ++item)
    {
      if (item->second == _texture)
      {
        delete _texture;
        mTextures.erase(item);
        break;
      }
    }
  }

  ITexture* DirectXRenderManager::getByName(const std::string& _name)
  {
    MapTexture::const_iterator item = mTextures.find(_name);
    return item == mTextures.end() ? nullptr : item->second;
  }

  IVertexBuffer* DirectXRenderManager::createVertexBuffer()
  {
    return new DirectXVertexBuffer(mpD3DDevice, this);
  }

  void DirectXRenderManager::setRenderQueueListener(IRenderQueueListener* _listener)
  {
  }

  void DirectXRenderManager::addResourceLocation(const std::string& _name, const std::string& _type, const std::string& _group, bool _recursive)
  {
	  ArhivInfo info;
	  info.name = _name;
	  info.group = _group;
	  info.recursive = _recursive;
	  mPaths.push_back(info);
  }

  const VectorString& DirectXRenderManager::getVectorResourcePath(
    const std::string& _pattern,
    const std::string& _group,
    bool _fullpath,
    bool _fullmatch)
  {
    static VectorString result;
    result.clear();

	for (VectorArhivInfo::iterator item=mPaths.begin(); item!=mPaths.end(); ++item)
	{
		if (_group == (*item).group)
		{
			scanFolder(result, (*item).name, (*item).recursive, _pattern, _fullpath);
		}
	}

    return result;
  }

} // namespace MyGUI
