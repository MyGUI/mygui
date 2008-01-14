/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_POINTER_MANAGER_H__
#define __MYGUI_POINTER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Platform.h"
#include "MyGUI_Instance.h"
#include <OgrePanelOverlayElement.h>
#include "MyGUI_LayerManager.h"
#include "MyGUI_PointerInfo.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_UnlinkWidget.h"

namespace MyGUI
{

	class _MyGUIExport PointerManager : public LayerItemInfo, public UnlinkWidget
	{
		INSTANCE_HEADER(PointerManager);

	public:
		void initialise();
		void shutdown();

	public:

		bool load(const std::string & _file, bool _resource = true);
		void clear();

		void _load(xml::xmlNodePtr _node, const std::string & _file);

		void show();
		void hide();

		inline bool isShow() {return mOverlayElement->isVisible();}

		void setPosition(const IntPoint& _pos);
		void setPointer(const std::string & _name, WidgetPtr _owner);

		inline void defaultPointer()
		{
			setPointer(mDefaultPointer, null);
		}

		void _unlinkWidget(WidgetPtr _widget);

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);

	private:
		std::string mDefaultPointer;
		std::string mLayer;
		std::string mMaterial;
		IntPoint mPoint;
		MapPointerInfo mMapPointers;
		PanelAlphaOverlayElement * mOverlayElement;
		WidgetPtr mWidgetOwner;

	}; // class PointerManager

} // namespace MyGUI

#endif // __MYGUI_POINTER_MANAGER_H__
