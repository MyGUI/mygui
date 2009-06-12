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
		MYGUI_RTTI_DERIVED( RTTLayerNode );

	public:
		explicit RTTLayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);
		virtual ~RTTLayerNode();

		// необходимо обновление нода
		virtual void outOfDate(RenderItem* _item);

		// добавляем айтем к ноду
		virtual void attachLayerItem(ILayerItem* _item);
		// удаляем айтем из нода
		virtual void detachLayerItem(ILayerItem* _item);

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		bool getCacheUsing() { return mChacheUsing; }
		void setCacheUsing(bool _value);

		bool getAnimate() { return mIsAnimate; }

		void setDestroy(bool _value) { mDestroy = _value; }
		bool getDestroy() { return mDestroy; }

		void addLayerNodeAnimation(LayerNodeAnimation* _impl);

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
		bool mDestroy;
		bool mIsAnimate;

		typedef std::vector<LayerNodeAnimation*> VectorLayerNodeAnimation;
		VectorLayerNodeAnimation mLayerNodeAnimation;

		VectorQuadData mData;
		QuadData mDefaultData;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_NODE_H__
