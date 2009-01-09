/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_DRAW_ITEM_H__
#define __MYGUI_DRAW_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	struct Vertex
	{
		float x, y, z;
		uint32 colour;
		float u, v;
	};

	class MYGUI_EXPORT DrawItem
	{

	public:
		virtual ~DrawItem() {};
		virtual size_t _drawItem(Vertex * _vertex, bool _update) = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_DRAW_ITEM_H__
