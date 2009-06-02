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
#ifndef __MYGUI_TEXTURE_LAYER_NODE_H__
#define __MYGUI_TEXTURE_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_ITexture.h"

namespace MyGUI
{

	class LayerItem;
	class TextureLayerNode;

	typedef std::vector<RenderItem*> VectorRenderItem;
	typedef std::vector<ILayerItem*> VectorLayerItem;
	typedef std::vector<TextureLayerNode*> VectorTextureLayerItemKeeper;
	typedef std::vector<IDrawItem*> VectorIDrawItem;

	class /*MYGUI_EXPORT */TextureLayerNode : public ILayerNode
	{
		MYGUI_RTTI_CHILD_HEADER ( TextureLayerNode, ILayerNode );

	public:
		explicit TextureLayerNode(ILayer* _layer, TextureLayerNode * _parent = nullptr);
		virtual ~TextureLayerNode();

		void _addUsing() { mCountUsing++; }
		void _removeUsing() { mCountUsing--; }
		size_t _countUsing() { return mCountUsing; }

		// возвращает колличество узлов
		size_t getItemCount();

		void renderToTarget(IRenderTarget* _target, bool _update);

		ILayerItem * getLayerItemByPoint(int _left, int _top);

		virtual void attachLayerItem(ILayerItem* _item);
		virtual void detachLayerItem(ILayerItem* _item);

		// обновляет очередь буферов
		//void _update();

		TextureLayerNode * getParent() { return mParent; }

		virtual ILayerNode* createItemNode();

		// удаляем дочку
		void destroyItemNode(TextureLayerNode* _item);

		// удаляем себя
		virtual void destroyItemNode();

		// поднимает свою дочку
		TextureLayerNode * upItemNode(TextureLayerNode* _item);

		// поднимает себя у родителей
		void upItemNode();

		bool existItemNode(TextureLayerNode* _item);

		virtual ILayer* getLayer();

		virtual RenderItem* addToRenderItem(const std::string& _texture, IDrawItem* _item);
		virtual void outOfDate(RenderItem* _item);

		//uint32* getTextureLock() { return mTexturePtr; }
		//int getWidth();
		//int getHeight();
		//int getLeft();
		//int getTop();

		//void outOfDate() { mOutOfDate = true; }

		//uint8* getLockTexture(ITexture* _texture);
		static bool msUseCashe;
		static bool msUpdate;

	private:
		RenderItem* addToRenderItem(const std::string& _texture, bool _first);

		void checkTexture();

	private:
		size_t mCountUsing;

		// список двух очередей отрисовки, для сабскинов и текста
		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;

		// список всех рутовых виджетов
		// у перекрывающегося слоя здесь только один
		//VectorLayerItem mLayerItems;
		ILayerItem* mLayerItem;

		// список такиж как мы, для построения дерева
		//VectorTextureLayerItemKeeper mChildItems;

		TextureLayerNode * mParent;
		ILayer* mLayer;

		IVertexBuffer* mVertexBuffer;
		ITexture* mTexture;

		// координата зю
		//static float mMaximumDepth;

		// размер пикселя в относительных координатах
		//static float mPixScaleX;
		//static float mPixScaleY;

		// смещение для того, чтобы тексель попал в пиксель
        //static float mHOffset;
        //static float mVOffset;

		//static float mAspectCoef;

		//IntSize mViewSize;
		//IntPoint mAbsolutePoint;
		IntSize mTextureSize;
		IntCoord mCurrentCoord;

		VectorIDrawItem mRenderItems;
		//uint32* mTexturePtr;

		bool mOutOfDate;

		//typedef std::pair<ITexture*, uint8*> TexturePair;
		//typedef std::vector<TexturePair> VectorITexture;
		//VectorITexture mLockTextures;
	};

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_LAYER_NODE_H__
