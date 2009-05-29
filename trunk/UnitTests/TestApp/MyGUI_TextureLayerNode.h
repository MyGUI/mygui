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
	//typedef std::vector<RenderItem*> VectorRenderItem;
	typedef std::vector<ILayerItem*> VectorLayerItem;
	typedef std::vector<TextureLayerNode*> VectorTextureLayerItemKeeper;
	typedef std::vector<IDrawItem*> VectorIDrawItem;

	class /*MYGUI_EXPORT */TextureLayerNode : public ILayerNode
	{
	public:
		explicit TextureLayerNode(ILayer* _layer, TextureLayerNode * _parent = nullptr);
		virtual ~TextureLayerNode();

		void _addUsing() { mCountUsing++; }
		void _removeUsing() { mCountUsing--; }
		size_t _countUsing() { return mCountUsing; }

		// ���������� ����������� �����
		size_t getItemCount();

		void _render(bool _update);

		ILayerItem * getLayerItemByPoint(int _left, int _top);

		virtual void attachLayerItem(ILayerItem* _root);
		virtual void detachLayerItem(ILayerItem* _root);

		// ��������� ������� �������
		void _update();

		TextureLayerNode * getParent() { return mParent; }

		virtual ILayerNode* createItemNode();

		// ������� �����
		void destroyItemNode(TextureLayerNode* _item);

		// ������� ����
		virtual void destroyItemNode();

		// ��������� ���� �����
		TextureLayerNode * upItemNode(TextureLayerNode* _item);

		// ��������� ���� � ���������
		void upItemNode();

		bool existItemNode(TextureLayerNode* _item);

		virtual ILayer* getLayer();

		virtual IRenderItem * addToRenderItem(const std::string& _texture, IDrawItem* _item);

	//private:
		//IRenderItem * addToRenderItem(const std::string& _texture, bool _first);

	//private:
		size_t mCountUsing;

		// ������ ���� �������� ���������, ��� ��������� � ������
		//VectorRenderItem mRenderItems;
		//VectorRenderItem mSecondRenderItems;

		// ������ ���� ������� ��������
		// � ���������������� ���� ����� ������ ����
		//VectorLayerItem mLayerItems;
		LayerItem* mLayerItem;

		// ������ ����� ��� ��, ��� ���������� ������
		//VectorTextureLayerItemKeeper mChildItems;

		TextureLayerNode * mParent;
		ILayer* mLayer;

		IVertexBuffer* mVertexBuffer;
		ITexture* mTexture;

		// ���������� ��
		static float mMaximumDepth;

		// ������ ������� � ������������� �����������
		static float mPixScaleX;
		static float mPixScaleY;

		// �������� ��� ����, ����� ������� ����� � �������
        static float mHOffset;
        static float mVOffset;

		static float mAspectCoef;

		IntSize mViewSize;

		VectorIDrawItem mRenderItems;
	};

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_LAYER_NODE_H__
