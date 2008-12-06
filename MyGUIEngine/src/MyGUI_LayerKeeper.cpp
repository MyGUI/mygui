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

	LayerKeeper::LayerKeeper(const std::string& _name, bool _overlapped, bool _peek) :
		mName(_name),
		mIsOverlapped(_overlapped),
		mIsPeek(_peek)
	{
	}

	LayerKeeper::~LayerKeeper()
	{
		MYGUI_ASSERT(mLayerItemKeepers.empty(), "Layer '" << mName << "' must be empty before destroy");
	}

	void LayerKeeper::_render(bool _update)
	{
		for (VectorLayerItemKeeper::iterator iter=mLayerItemKeepers.begin(); iter!=mLayerItemKeepers.end(); ++iter) {
			(*iter)->_render(_update);
		}
	}

	LayerItemKeeper * LayerKeeper::getItem()
	{
		LayerItemKeeper * layer = 0;

		if ((mIsOverlapped) || (mLayerItemKeepers.empty())) {
			layer = new LayerItemKeeper();
			mLayerItemKeepers.push_back(layer);
		}
		else {
			layer = mLayerItemKeepers.front();
		}

		layer->_addUsing();
		return layer;
	}

	void LayerKeeper::leaveItem(LayerItemKeeper * _item)
	{
		for (VectorLayerItemKeeper::iterator iter=mLayerItemKeepers.begin(); iter!=mLayerItemKeepers.end(); ++iter) {
			if ((*iter) == _item) {
				_item->_removeUsing();
				if (0 == _item->_countUsing()) {
					delete _item;
					mLayerItemKeepers.erase(iter);
				}
				return;
			}
		}
		MYGUI_EXCEPT("item keeper not found");
	}

	void LayerKeeper::upItem(LayerItemKeeper * _item)
	{
		if ((2 > mLayerItemKeepers.size()) || (mLayerItemKeepers.back() == _item)) return;
		for (VectorLayerItemKeeper::iterator iter=mLayerItemKeepers.begin(); iter!=mLayerItemKeepers.end(); ++iter) {
			if ((*iter) == _item) {
				mLayerItemKeepers.erase(iter);
				mLayerItemKeepers.push_back(_item);
				break;
			}
		}
	}

	LayerItem * LayerKeeper::_findLayerItem(int _left, int _top, LayerItem* &_root)
	{
		if (false == mIsPeek) return null;
		VectorLayerItemKeeper::reverse_iterator iter = mLayerItemKeepers.rbegin();
		while (iter != mLayerItemKeepers.rend()) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top, _root);
			if (item != null) return item;
			++iter;
		}
		return null;
	}

	bool LayerKeeper::existItem(LayerItemKeeper * _item)
	{
		for (VectorLayerItemKeeper::iterator iter=mLayerItemKeepers.begin(); iter!=mLayerItemKeepers.end(); ++iter) {
			if ((*iter) == _item) return true;
		}
		return false;
	}

} // namespace MyGUI
