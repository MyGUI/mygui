/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_LayerKeeper.h"
#include "MyGUI_LayerItemKeeper.h"

namespace MyGUI
{

	LayerKeeper::LayerKeeper(const std::string& _name, bool _overlapped) :
		mName(_name),
		mIsOverlapped(_overlapped)
	{
	}

	void LayerKeeper::_render()
	{
		for (VectorLayerItemKeeper::iterator iter=mLayerItemKeepers.begin(); iter!=mLayerItemKeepers.end(); ++iter) {
			(*iter)->_render();
		}
	}

	LayerItemKeeper * LayerKeeper::getItem()
	{
		LayerItemKeeper * layer = 0;

		if ((mIsOverlapped) || (mLayerItemKeepers.empty())) layer = new LayerItemKeeper();
		else layer = mLayerItemKeepers.front();

		mLayerItemKeepers.push_back(layer);
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
				break;
			}
		}
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

	void LayerKeeper::_resize(const FloatSize& _size)
	{
		for (VectorLayerItemKeeper::iterator iter=mLayerItemKeepers.begin(); iter!=mLayerItemKeepers.end(); ++iter) {
			(*iter)->_resize(_size);
		}
	}

} // namespace MyGUI
