/*!
	@file
	@author		Albert Semenov
	@date		12/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DirectXRTTexture.h"

#include <d3dx9.h>

namespace MyGUI
{

	DirectXRTTexture::DirectXRTTexture(IDirect3DTexture9* _texture) :
		mpTexture(_texture)
	{
	}

	DirectXRTTexture::~DirectXRTTexture()
	{
	}

	void DirectXRTTexture::begin()
	{
	}

	void DirectXRTTexture::end()
	{
	}

	void DirectXRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
	}

} // namespace MyGUI
