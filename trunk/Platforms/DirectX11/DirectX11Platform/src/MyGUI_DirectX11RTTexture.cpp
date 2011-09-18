/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#include <d3dx11.h>
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11RTTexture.h"
#include "MyGUI_DirectX11RenderManager.h"

namespace MyGUI
{

	DirectX11RTTexture::DirectX11RTTexture( DirectX11Texture* _texture ) : mTexture(_texture)
	{
		int width = mTexture->getWidth();
		int height = mTexture->getHeight();

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	DirectX11RTTexture::~DirectX11RTTexture()
	{
	}

	void DirectX11RTTexture::begin()
	{
	}

	void DirectX11RTTexture::end()
	{
	}

	void DirectX11RTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DirectX11RenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
