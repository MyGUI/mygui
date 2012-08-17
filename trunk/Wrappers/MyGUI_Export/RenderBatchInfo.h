/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _306b8f50_76ed_4926_bc72_ace3a3d8f10a_
#define _306b8f50_76ed_4926_bc72_ace3a3d8f10a_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ExportTexture.h"
#include "MyGUI_ExportVertexBuffer.h"
#include "MyGUI_ILayer.h"

namespace MyGUI
{

	struct RenderBatchInfo
	{
		RenderBatchInfo() :
			Buffer(nullptr),
			Texture(nullptr),
			Count(0),
			Layer(nullptr)
		{
		}

		RenderBatchInfo(ExportVertexBuffer* _buffer, ExportTexture* _texture, size_t _count, ILayer* _layer) :
			Buffer(_buffer),
			Texture(_texture),
			Count(_count),
			Layer(_layer)
		{
		}

		ExportVertexBuffer* Buffer;
		ExportTexture* Texture;
		size_t Count;
		ILayer* Layer;
	};

}

#endif
