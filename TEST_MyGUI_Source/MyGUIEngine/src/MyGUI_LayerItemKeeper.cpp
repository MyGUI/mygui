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

	void LayerItemKeeper::_render()
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			(*iter)->_render();
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			(*iter)->_render();
		}
	}

	RenderItem * LayerItemKeeper::addToRenderItem(const std::string& _texture, bool _first, bool _separate)
	{
		// для первичной очереди нужен порядок
		if (_first) {
			if (_separate || (mFirstRenderItems.empty()) || (mFirstRenderItems.back()->getTextureName() != _texture)) {
				RenderItem * item = new RenderItem(_texture);
				mFirstRenderItems.push_back(item);
			}
			return mFirstRenderItems.back();
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			if ((*iter)->getTextureName() == _texture) return (*iter);
		}
		// не найденно создадим новый
		mSecondRenderItems.push_back(new RenderItem(_texture));
		return mSecondRenderItems.back();
	}

	void LayerItemKeeper::_resize(const FloatSize& _size)
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			(*iter)->_resize(_size);
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			(*iter)->_resize(_size);
		}
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

} // namespace MyGUI
