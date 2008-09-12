/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_INTERFACE_H__
#define __MYGUI_SUB_WIDGET_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_DrawItem.h"

namespace MyGUI
{

	// базовый тип информации о стейте
	typedef void StateInfo;
	typedef std::vector<StateInfo*> VectorStateInfo;
	typedef std::map<std::string, StateInfo*> MapStateInfo;
	typedef std::map<std::string, VectorStateInfo> MapWidgetStateInfo;

	class SubWidgetInterface;
	typedef std::vector<SubWidgetInterface*> VectorSubWidget;

	class LayerItemKeeper;
	class RenderItem;


	class _MyGUIExport SubWidgetInterface : public CroppedRectangleInterface, public DrawItem
	{
	public:
		SubWidgetInterface(const IntCoord & _coord, Align _align, CroppedRectangleInterface * _parent) : 
			CroppedRectangleInterface(_coord, _align, _parent)
		{ }

		virtual void _setUVSet(const FloatRect& _rect) { }
		virtual void _setStateData(void * _data) { }

		virtual bool _isText() { return false; }

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item) { }
		virtual void _destroyDrawItem() { }

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_INTERFACE_H__
