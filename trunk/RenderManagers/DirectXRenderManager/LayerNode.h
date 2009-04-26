/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __MYGUI_LAYER_NODE_H__
#define __MYGUI_LAYER_NODE_H__

#include "MyGUI_ILayerNode.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_ILayerItem.h"
#include "RenderItem.h"

#include <vector>

namespace MyGUI
{

	class LayerNode : public ILayerNode
	{
	public:
		LayerNode(ILayer* _layer) : mLayer(_layer) { }
		virtual ~LayerNode() { }

		// ����, �������� �� �����������
		virtual ILayer* getLayer() { return mLayer; }

		// ������� �������� ���
		virtual ILayerNode* createItemNode();

		// ������� ���� � ��� �������� ����
		virtual void destroyItemNode();

		// ��������� ����� � ����
		virtual void attachLayerItem(ILayerItem* _item);
		// ������� ����� �� ����
		virtual void detachLayerItem(ILayerItem* _root);

		// ��������� ���� ��� ������� ������
		virtual void upItemNode();

		// ��������� ��� ����� � ���������� ������ �����
		virtual IRenderItem* addToRenderItem(const std::string& _texture, IDrawItem* _item);

		void doRender(bool _update);

	private:
		ILayer* mLayer;

		typedef std::vector<ILayerItem*> VectorLayerItem;
		VectorLayerItem mItems;

		typedef std::vector<RenderItem*> VectorRenderItem;
		VectorRenderItem mRenderItem;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_NODE_H__
