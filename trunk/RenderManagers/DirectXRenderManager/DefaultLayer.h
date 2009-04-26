/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __MYGUI_DEFAULT_LAYER_H__
#define __MYGUI_DEFAULT_LAYER_H__

#include "MyGUI_ILayer.h"
#include "LayerNode.h"
#include <vector>

namespace MyGUI
{

	class DefaultLayer : public ILayer
	{
	public:
		DefaultLayer(const std::string& _name) : ILayer(_name) { }
		virtual ~DefaultLayer() { }


		// ������� ����
		virtual ILayerNode* createItemNode(ILayerNode* _parent);
		// ������� ����
		virtual void destroyItemNode(ILayerNode* _item);
		// ���������� �� ����
		virtual bool existItemNode(ILayerNode* _item);
		// ��������� ���� ��� ������ �������������
		virtual void upItemNode(ILayerNode* _item);

		// ������ ����
		virtual void doRender(bool _update);

		// ���������� ������ �� �������
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

	private:
		typedef std::vector<LayerNode*> VectorLayerNode;
		VectorLayerNode mNodes;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_H__
