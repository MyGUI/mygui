#ifndef __BASIS_WIDGET_MANAGER_H__
#define __BASIS_WIDGET_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "BasisWidget.h"
#include "BasisWidgetFactoryInterface.h"

#include "MainSkin.h"
#include "SubSkin.h"
#include "TextSimple.h"
#include "TextEdit.h"

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

#endif // __BASIS_WIDGET_MANAGER_H__