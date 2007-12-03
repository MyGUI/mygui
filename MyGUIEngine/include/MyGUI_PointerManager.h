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

namespace MyGUI
{

	class _MyGUIExport PointerManager : public LayerItemInfo
	{
		INSTANCE_HEADER(PointerManager);

	public:
		void initialise();
		void shutdown();

	public:

		bool load(const std::string & _file, bool _resource = true);
		void clear();

		void show(bool _show = true);
		inline void hide() {show(false);}
		inline bool isShow() {return mShow;}

		void setPosition(int _left, int _top);

		void setPointer(const std::string & _name, WidgetPtr _owner);

		inline void defaultPointer()
		{
			setPointer(mDefaultPointer, null);
		}

		inline void unlinkWidget(WidgetPtr _widget)
		{
			if (_widget == mWidgetOwner) defaultPointer();
		}

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);

	private:
		std::string mDefaultPointer;
		std::string mLayer;
		std::string mMaterial;
		bool mShow;
		IntPoint mPoint;
		MapPointerInfo mMapPointers;
		PanelAlphaOverlayElement * mOverlayElement;
		WidgetPtr mWidgetOwner;

	}; // class PointerManager

} // namespace MyGUI

#endif // __MYGUI_POINTER_MANAGER_H__