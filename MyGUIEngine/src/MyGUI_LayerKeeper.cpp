/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_LayerKeeper.h"
#include "MyGUI_LayerItemKeeper.h"

namespace MyGUI
{

	LayerKeeper::LayerKeeper(const std::string& _name, bool _overlapped, bool _pick) :
		mName(_name),
		mIsOverlapped(_overlapped),
		mIsPeek(_pick)
	{
	}

	LayerKeeper::~LayerKeeper()
	{
		MYGUI_ASSERT(mChildItems.empty(), "Layer '" << mName << "' must be empty before destroy");
	}

	void LayerKeeper::_render(bool _update)
	{
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
	}

	LayerItemKeeper * LayerKeeper::createItem(LayerItemKeeper * _parent)
	{
		// пусть парент сам рулит
		if (_parent) {
			return _parent->createItem();
		}

		// создаем рутовый айтем
		LayerItemKeeper * layer = 0;

		if ((mIsOverlapped) || (mChildItems.empty())) {
			layer = new LayerItemKeeper(this);
			mChildItems.push_back(layer);
		}
		else {
			layer = mChildItems.front();
		}

		layer->_addUsing();
		return layer;
	}

	void LayerKeeper::destroyItem(LayerItemKeeper * _item)
	{
		LayerItemKeeper * parent = _item->getParent();
		// если есть отец, то русть сам и удаляет
		if (parent) {
			parent->destroyItem(_item);
		}
		// айтем рутовый, мы удаляем
		else {
			for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
				if ((*iter) == _item) {
					_item->_removeUsing();
					if (0 == _item->_countUsing()) {
						delete _item;
						mChildItems.erase(iter);
					}
					return;
				}
			}
			MYGUI_EXCEPT("item keeper not found");
		}
	}

	void LayerKeeper::upItem(LayerItemKeeper * _item)
	{
		LayerItemKeeper * parent = _item->getParent();
		// если есть отец, то пусть сам рулит
		if (parent) {
			// возвращается рутовый айтем
			_item = parent->upItem(_item);
		}

		if ((2 > mChildItems.size()) || (mChildItems.back() == _item)) return;
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			if ((*iter) == _item) {
				mChildItems.erase(iter);
				mChildItems.push_back(_item);
				return;
			}
		}

		MYGUI_EXCEPT("item keeper not found");
	}

	LayerItem * LayerKeeper::_findLayerItem(int _left, int _top)
	{
		if (false == mIsPeek) return null;
		VectorLayerItemKeeper::reverse_iterator iter = mChildItems.rbegin();
		while (iter != mChildItems.rend()) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top);
			if (item != null) return item;
			++iter;
		}
		return null;
	}

	bool LayerKeeper::existItem(LayerItemKeeper * _item)
	{
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			if ((*iter) == _item || (*iter)->existItem(_item)) return true;
		}
		return false;
	}

	size_t LayerKeeper::getItemCount()
	{
		size_t count = 0;
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			count += (*iter)->getItemCount();
		}
		return count;
	}

	size_t LayerKeeper::getSubItemCount()
	{
		size_t count = 0;
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			count += (*iter)->getItemCount();
		}
		return count - mChildItems.size();
	}

} // namespace MyGUI
