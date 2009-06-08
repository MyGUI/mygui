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
#include <d3dx9.h>

namespace MyGUI
{

  MYGUI_INSTANCE_IMPLEMENT(DirectXRenderManager);

  void DirectXRenderManager::initialise(IDirect3DDevice9 *_device)
  {
    // new
    mpD3DDevice = _device;

    //mTexelOffset.set(0, 0);
    mMaximumDepth = 0.0f;
    mVertexFormat = VertexColourType::ColourABGR;  

    memset(&mInfo, 0, sizeof(mInfo));
    if (mpD3DDevice != nullptr)
    {
    D3DVIEWPORT9 vp;
    _device->GetViewport(&vp);
    mViewSize.set(vp.Width, vp.Height);
    
      mInfo.maximumDepth = mMaximumDepth;//mRenderSystem->getMaximumDepthInputValue();
      mInfo.hOffset = -0.5f / float(mViewSize.width);//mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
      mInfo.vOffset = -0.5f / float(mViewSize.height);//mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
      mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
      mInfo.pixScaleX = 1.0 / float(mViewSize.width);
      mInfo.pixScaleY = 1.0 / float(mViewSize.height);
    }

    mInfo.rttFlipY = false;

    mUpdate = false;
    mListener = nullptr;
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

  void DirectXRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
  {
    delete _buffer;
  }

  void DirectXRenderManager::setRenderQueueListener(IRenderQueueListener* _listener)
  {
    mListener = _listener;
    mUpdate = true;
  }

  void DirectXRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
  {
    DirectXTexture *dxTex = static_cast<DirectXTexture*>(_texture);
    dxTex->bindToStage(0);
    DirectXVertexBuffer *dxVB = static_cast<DirectXVertexBuffer*>(_buffer);
    dxVB->setToStream(0);
    // count in vertexes, triangle_list = vertexes / 3
    mpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _count / 3);
  }

  void DirectXRenderManager::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
  {
    ITexture *texture = getByName(_texture);
    doRender(_buffer, texture, _count);
  }

  void DirectXRenderManager::render()
  {
    if (mListener != nullptr) 
    {
      begin();
      mListener->doRender(mUpdate);
      end();

    }
    mUpdate = false;
  }

  void DirectXRenderManager::begin()
  {
    mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    mpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    mpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_DIFFUSE);
    mpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

    mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
    mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    mpD3DDevice->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
    mpD3DDevice->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);

    mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    mpD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    mpD3DDevice->SetRenderState(D3DRS_LIGHTING, 0);
    mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    //mpD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    mpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    D3DXMATRIX m;
    D3DXMatrixIdentity(&m);
    mpD3DDevice->SetTransform(D3DTS_WORLD, &m);
    mpD3DDevice->SetTransform(D3DTS_VIEW, &m);
    mpD3DDevice->SetTransform(D3DTS_PROJECTION, &m);
  }

} // namespace MyGUI
