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


		// создает узел
		virtual ILayerNode* createItemNode(ILayerNode* _parent);
		// удаляет узел
		virtual void destroyItemNode(ILayerNode* _item);
		// существует ли узел
		virtual bool existItemNode(ILayerNode* _item);
		// поднимает узел над своими родствениками
		virtual void upItemNode(ILayerNode* _item);

		// рисует леер
		virtual void doRender(bool _update);

		// возвращает виджет по позиции
		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

	private:
		typedef std::vector<LayerNode*> VectorLayerNode;
		VectorLayerNode mNodes;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_H__
