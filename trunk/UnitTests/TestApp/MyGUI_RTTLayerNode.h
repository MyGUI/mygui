/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#ifndef __MYGUI_RTT_LAYER_NODE_H__
#define __MYGUI_RTT_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_LayerNodeAnimation.h"

namespace MyGUI
{

	class /*MYGUI_EXPORT */RTTLayerNode : public LayerNode
	{
		MYGUI_RTTI_DERIVED ( RTTLayerNode );

	public:
		explicit RTTLayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);
		virtual ~RTTLayerNode();

		// ���������� ���������� ����
		virtual void outOfDate(RenderItem* _item);

		// ��������� ����� � ����
		virtual void attachLayerItem(ILayerItem* _item);
		// ������� ����� �� ����
		virtual void detachLayerItem(ILayerItem* _item);

		// ������ ����
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		bool getCacheUsing() { return mChacheUsing; }
		void setCacheUsing(bool _value);

		bool getDelayDestroy() { return mDelayDestroy; }
		void setDelayDestroy(bool _value) { mDelayDestroy = _value; }

		void setLayerNodeAnimation(LayerNodeAnimation* _impl) { mLayerNodeAnimation = _impl; mMajorUpdate = true; }
		LayerNodeAnimation* getLayerNodeAnimation() { return mLayerNodeAnimation; }

	private:
		void checkTexture();

	private:
		IVertexBuffer* mVertexBuffer;
		ITexture* mTexture;

		IntSize mTextureSize;
		IntCoord mCurrentCoord;

		bool mOutOfDate;

		bool mMajorUpdate;
		bool mChacheUsing;
		bool mDelayDestroy;

		LayerNodeAnimation* mLayerNodeAnimation;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_NODE_H__
