/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_RenderItem.h"

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
		if (_first) {
			if (_separate || (mFirstRenderItems.empty()) || (mFirstRenderItems.back()->getTextureName() != _texture)) {
				RenderItem * item = new RenderItem(_texture);
				mFirstRenderItems.push_back(item);
			}
			return mFirstRenderItems.back();
		}

		if (_separate || (mSecondRenderItems.empty()) || (mSecondRenderItems.back()->getTextureName() != _texture)) {
			RenderItem * item = new RenderItem(_texture);
			mSecondRenderItems.push_back(item);
		}
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

} // namespace MyGUI
