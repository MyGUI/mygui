/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_MANAGER_H__
#define __MYGUI_CONTROLLER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_ControllerItem.h"
#include "MyGUI_UnlinkWidget.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ActionController.h"

namespace MyGUI
{

	typedef std::pair<WidgetPtr, ControllerItem *> PairControllerItem;
	typedef std::list<PairControllerItem> ListControllerItem;

	class _MyGUIExport ControllerManager : public FrameListener, public UnlinkWidget
	{
		INSTANCE_HEADER(ControllerManager);

	public:
		void initialise();
		void shutdown();

		/** Add controlled widget
			@param
				_widget to be controlled
			@param
				_item controller with some actions (for example ControllerFadeAlpha or your own)
			@note _item will be deleted automatically at end of controller lifetime (or by removeItem)
		*/
		void addItem(WidgetPtr _widget, ControllerItem * _item);
		/** Stop the control over a widget
			@param
				_widget to be removed
		*/
		void removeItem(WidgetPtr _widget);

		void _unlinkWidget(WidgetPtr _widget);

	private:
		void _frameEntered(float _time);
		void clear();

	private:
		ListControllerItem mListItem;

	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_MANAGER_H__
