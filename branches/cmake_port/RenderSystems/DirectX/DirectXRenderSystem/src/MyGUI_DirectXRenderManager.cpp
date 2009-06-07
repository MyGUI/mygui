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

	void DirectXRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
	}

	void DirectXRenderManager::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
	{
	}

} // namespace MyGUI
