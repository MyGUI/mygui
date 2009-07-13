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
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_OpenGLTextureManager.h"
#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_OpenGLDiagnostic.h"

#include <d3dx9.h>

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OpenGLRenderManager);

	void OpenGLRenderManager::initialise(IDirect3DDevice9 *_device)
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mpD3DDevice = _device;

		mVertexFormat = VertexColourType::ColourABGR;  

		memset(&mInfo, 0, sizeof(mInfo));
		if (mpD3DDevice != nullptr)
		{
			D3DVIEWPORT9 vp;
			_device->GetViewport(&vp);
			mViewSize.set(vp.Width, vp.Height);

			mInfo.maximumDepth = 0.0f;
			mInfo.hOffset = -0.5f / float(mViewSize.width);
			mInfo.vOffset = -0.5f / float(mViewSize.height);
			mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
			mInfo.pixScaleX = 1.0 / float(mViewSize.width);
			mInfo.pixScaleY = 1.0 / float(mViewSize.height);
		}

		mInfo.rttFlipY = false;

		mUpdate = false;
		mListener = nullptr;

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLRenderManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mpD3DDevice = nullptr;

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGLRenderManager::createVertexBuffer()
	{
		return new OpenGLVertexBuffer(mpD3DDevice, this);
	}

	void OpenGLRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OpenGLRenderManager::setRenderQueueListener(IRenderQueueListener* _listener)
	{
		mListener = _listener;
		mUpdate = true;
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLTexture *dxTex = static_cast<OpenGLTexture*>(_texture);
		dxTex->bindToStage(0);
		OpenGLVertexBuffer *dxVB = static_cast<OpenGLVertexBuffer*>(_buffer);
		dxVB->setToStream(0);
		// count in vertexes, triangle_list = vertexes / 3
		mpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _count / 3);
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
	{
		ITexture *texture = TextureManager::getInstance().getByName(_texture);
		doRender(_buffer, texture, _count);
	}

	void OpenGLRenderManager::render()
	{
		if (mListener != nullptr) 
		{
			begin();
			mListener->doRender(mUpdate);
			end();
		}
		mUpdate = false;
	}

	void OpenGLRenderManager::begin()
	{
		mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		mpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		mpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_DIFFUSE);
		mpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		mpD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		mpD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		mpD3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

		mpD3DDevice->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
		mpD3DDevice->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);

		mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		mpD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		mpD3DDevice->SetRenderState(D3DRS_LIGHTING, 0);
		mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		mpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		mpD3DDevice->SetTransform(D3DTS_WORLD, &m);
		mpD3DDevice->SetTransform(D3DTS_VIEW, &m);
		mpD3DDevice->SetTransform(D3DTS_PROJECTION, &m);
	}

} // namespace MyGUI
