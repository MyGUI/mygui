/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#include "QuadDrawItem.h"

namespace MyGUI
{

	QuadDrawItem::QuadDrawItem() :
		mRenderItem(0)
	{
	}

	void QuadDrawItem::createDrawItem(const std::string& _texture, ILayerNode* _node)
	{
		// создаем для себя данные и запрашиваем колличество вершин
		IRenderItem* item = _node->addToRenderItem(_texture, this);
		mRenderItem = static_cast<RenderItem*>(item);
		mRenderItem->addDrawItem(this, 6);
	}

	void QuadDrawItem::destroyDrawItem()
	{
	}

	void QuadDrawItem::doRender(bool _update)
	{
		// а вот тут уе рисуем все что нам надо
		// главное проверять, чтобы мы не зашли за границу, которую запрашивали
		int* buff = mRenderItem->getBuffer();
	}

} // namespace MyGUI
