/*!
	@file
	@author		Albert Semenov
	@date		02/2008
*/

#ifndef MYGUI_RTT_LAYER_NODE_H_
#define MYGUI_RTT_LAYER_NODE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_LayerNodeAnimation.h"
#include <MyGUI_Timer.h>

namespace MyGUI
{

	class /*MYGUI_EXPORT */ RTTLayerNode : public LayerNode
	{
		MYGUI_RTTI_DERIVED(RTTLayerNode)

	public:
		explicit RTTLayerNode(ILayer* _layer, ILayerNode* _parent = nullptr);
		~RTTLayerNode() override;

		// необходимо обновление нода
		void outOfDate(RenderItem* _item) override;

		// добавляем айтем к ноду
		void attachLayerItem(ILayerItem* _item) override;
		// удаляем айтем из нода
		void detachLayerItem(ILayerItem* _item) override;

		// рисует леер
		void renderToTarget(IRenderTarget* _target, bool _update) override;

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
		IVertexBuffer* mVertexBuffer{nullptr};
		ITexture* mTexture{nullptr};

		IntSize mTextureSize;
		IntCoord mCurrentCoord;

		bool mOutOfDateRtt{false};

		bool mMajorUpdate{false};
		bool mChacheUsing{true};
		bool mDestroy{false};
		bool mIsAnimate{false};

		using VectorLayerNodeAnimation = std::vector<LayerNodeAnimation*>;
		VectorLayerNodeAnimation mLayerNodeAnimation;

		VectorQuadData mData;
		QuadData mDefaultData;

		MyGUI::Timer mTimer;
	};

} // namespace MyGUI

#endif // MYGUI_RTT_LAYER_NODE_H_
