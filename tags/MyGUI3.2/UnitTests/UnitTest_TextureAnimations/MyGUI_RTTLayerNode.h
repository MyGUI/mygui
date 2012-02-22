/*!
	@file
	@author		Albert Semenov
	@date		02/2008
*/

#ifndef __MYGUI_RTT_LAYER_NODE_H__
#define __MYGUI_RTT_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_LayerNodeAnimation.h"
#include <MyGUI_Timer.h>

namespace MyGUI
{

	class /*MYGUI_EXPORT */RTTLayerNode :
		public LayerNode
	{
		MYGUI_RTTI_DERIVED( RTTLayerNode )

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

		bool getCacheUsing() const
		{
			return mChacheUsing;
		}
		void setCacheUsing(bool _value);

		bool getAnimate() const
		{
			return mIsAnimate;
		}

		void setDestroy(bool _value)
		{
			mDestroy = _value;
		}
		bool getDestroy() const
		{
			return mDestroy;
		}

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

		MyGUI::Timer mTimer;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_NODE_H__
