/*!
	@file
	@author		Albert Semenov
	@date		06/2008
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
#ifndef __MYGUI_LAYER_NODE_ANIMATION_H__
#define __MYGUI_LAYER_NODE_ANIMATION_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderTargetInfo.h"
#include "MyGUI_Serializable.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */LayerNodeAnimation : public Serializable
	{
		MYGUI_RTTI_DERIVED( LayerNodeAnimation );

	public:
		LayerNodeAnimation() { }
		virtual ~LayerNodeAnimation() { }

		virtual void attach(ILayerNode* _node) = 0;

		// �������� �����
		virtual void create() = 0;

		// ��������� �����
		virtual void destroy() = 0;

		// ����������� ������
		virtual size_t animate(bool _update, float _time, IVertexBuffer* _buffer, ITexture* _texture, const RenderTargetInfo& _info, const IntCoord& _coord) = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_NODE_ANIMATION_H__
