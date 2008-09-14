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
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_ISubWidgetFactory.h"

#include "MyGUI_SubSkin.h"
#include "MyGUI_MainSkin.h"
#include "MyGUI_SimpleText.h"
#include "MyGUI_EditText.h"
#include "MyGUI_RawRect.h"
#include "MyGUI_TileRect.h"

namespace MyGUI
{

	class _MyGUIExport SubWidgetManager
	{
		INSTANCE_HEADER(SubWidgetManager);

	public:
		void initialise();
		void shutdown();

		// создает сабвиджет использу€ фабрику
		ISubWidget * createSubWidget(const SubWidgetInfo &_info, ICroppedRectangle * _parent);

		inline void registerFactory(ISubWidgetFactory * _factory)
		{
			mFactoryList.push_back(_factory);
		}

		StateInfo * getStateData(const std::string & _factory, xml::xmlNodePtr _node, xml::xmlNodePtr _root);

	protected:
		std::list<ISubWidgetFactory*> mFactoryList;

		SubWidgetFactory<SubSkin> * mFactorySubSkin;
		SubWidgetFactory<MainSkin> * mFactoryMainSkin;
		SubWidgetFactory<SimpleText> * mFactorySimpleText;
		SubWidgetFactory<EditText> * mFactoryEditText;
		SubWidgetFactory<RawRect> * mFactoryRawRect;
		SubWidgetFactory<TileRect> * mFactoryTileRect;
		
	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_MANAGER_H__
