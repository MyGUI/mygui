/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_MANAGER_H__
#define __MYGUI_SUB_WIDGET_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_SubWidgetFactoryInterface.h"

#include "MyGUI_SubSkin.h"
#include "MyGUI_MainSkin.h"

namespace MyGUI
{
	class _MyGUIExport SubWidgetManager
	{
		INSTANCE_HEADER(SubWidgetManager);

	public:
		void initialise();
		void shutdown();

		// создает сабвиджет использу€ фабрику
		CroppedRectangleInterface * createSubWidget(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent);

		inline void registerFactory(SubWidgetFactoryInterface * _factory)
		{
			mFactoryList.push_back(_factory);
		}

	protected:
		std::list<SubWidgetFactoryInterface*> mFactoryList;

		CroppedRectangleFactory<SubSkin> * mFactorySubSkin;
		CroppedRectangleFactory<MainSkin> * mFactoryMainSkin;
		
	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_MANAGER_H__
