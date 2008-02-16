/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_KEEPER_H__
#define __MYGUI_LAYER_ITEM_KEEPER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class RenderItem;
	typedef std::vector<RenderItem*> VectorRenderItem;

	class _MyGUIExport LayerItemKeeper
	{
	public:
		LayerItemKeeper();
		~LayerItemKeeper();

		inline void _addUsing() { mCountUsing++; }
		inline void _removeUsing() { mCountUsing--; }
		inline size_t _countUsing() { return mCountUsing; }

		void _render();
		void _resize(const FloatSize& _size);

		RenderItem * addToRenderItem(const std::string& _texture, bool _first, bool _separate);
		//RenderItem * insertToRenderItem(const std::string& _texture, bool _first);

	private:
		size_t mCountUsing;

		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_KEEPER_H__
