/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BASIS_WIDGET_MANAGER_H__
#define __MYGUI_BASIS_WIDGET_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_BasisWidget.h"
#include "MyGUI_BasisWidgetFactoryInterface.h"

#include "MyGUI_MainSkin.h"
#include "MyGUI_SubSkin.h"
#include "MyGUI_TextSimple.h"
#include "MyGUI_TextEdit.h"

namespace MyGUI
{
	class _MyGUIExport BasisWidgetManager
	{
		INSTANCE_HEADER(BasisWidgetManager);

	public:
		void initialise();
		void shutdown();

		BasisWidget * createBasisWidget(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidget * _parent);

		inline void registerFactory(BasisWidgetFactoryInterface * _factory)
		{
			mFactoryList.push_back(_factory);
		}

	protected:
		std::list<BasisWidgetFactoryInterface*> mFactoryList;

		BasisWidgetFactory<MainSkin> * mFactoryMainSkin;
		BasisWidgetFactory<SubSkin> * mFactorySubSkin;
		BasisWidgetFactory<TextSimple> * mFactoryTextSimple;
		BasisWidgetFactory<TextEdit> * mFactoryTextEdit;

	}; // BasisWidgetManager

} // namespace MyGUI

#endif // __MYGUI_BASIS_WIDGET_MANAGER_H__