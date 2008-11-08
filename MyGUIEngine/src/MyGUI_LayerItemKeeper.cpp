/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItem.h"

namespace MyGUI
{

	LayerItemKeeper::LayerItemKeeper() :
		mCountUsing(0)
	{
	}

	LayerItemKeeper::~LayerItemKeeper()
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			delete (*iter);
		}
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			delete (*iter);
		}
		mSecondRenderItems.clear();
	}

	void LayerItemKeeper::_render(bool _update)
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
	}

	RenderItem * LayerItemKeeper::addToRenderItem(const std::string& _texture, bool _first, bool _separate)
	{
		// для первичной очереди нужен порядок
		if (_first) {
			if (mFirstRenderItems.empty()) {

				// создаем новый буфер
				RenderItem * item = new RenderItem(_texture, this);
				mFirstRenderItems.push_back(item);

				return item;
			}

			// если последний буфер пустой, то мона не создавать
			if (mFirstRenderItems.back()->getNeedVertexCount() == 0) {
				// пустых может быть сколько угодно, нужен самый первый из пустых
				for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
					if ((*iter)->getNeedVertexCount() == 0) {
						(*iter)->setTextureName(_texture);
						return (*iter);
					}
				}
			}

			// та же текстура
			if (mFirstRenderItems.back()->getTextureName() == _texture) {
				return mFirstRenderItems.back();
			}

			// создаем новый буфер
			RenderItem * item = new RenderItem(_texture, this);
			mFirstRenderItems.push_back(item);

			return item;
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			// либо такая же текстура, либо пустой буфер
			if ((*iter)->getTextureName() == _texture) {
				return (*iter);
			}
			else if ((*iter)->getNeedVertexCount() == 0) {
				(*iter)->setTextureName(_texture);
				return (*iter);
			}
			
		}
		// не найденно создадим новый
		mSecondRenderItems.push_back(new RenderItem(_texture, this));
		return mSecondRenderItems.back();
	}

	LayerItem * LayerItemKeeper::_findLayerItem(int _left, int _top, LayerItem* &_root)
	{
		for (VectorLayerItem::iterator iter=mPeekLayerItems.begin(); iter!=mPeekLayerItems.end(); ++iter) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top);
			if (null != item) {
				_root = (*iter);
				return item;
			}
		}

		return null;
	}

	void LayerItemKeeper::_update()
	{
		// буферы освобождаются по одному всегда

		if (mFirstRenderItems.size() > 1) {
			// пытаемся поднять пустой буфер выше полных
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end()) {
				if ((*iter1)->getNeedVertexCount() == 0) {
					RenderItem * tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
		}
	}

} // namespace MyGUI
