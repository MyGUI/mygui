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

namespace MyGUI
{

	struct RenderBatchInfo
	{
		RenderBatchInfo() :
			Buffer(nullptr),
			Texture(nullptr),
			Count(0)
		{
		}

		RenderBatchInfo(ExportVertexBuffer* _buffer, ExportTexture* _texture, size_t _count) :
			Buffer(_buffer),
			Texture(_texture),
			Count(_count)
		{
		}

		ExportVertexBuffer* Buffer;
		ExportTexture* Texture;
		size_t Count;
	};

}

#endif
