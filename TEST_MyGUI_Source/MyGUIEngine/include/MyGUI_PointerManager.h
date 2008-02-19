/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_POINTER_MANAGER_H__
#define __MYGUI_POINTER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_PointerInfo.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_UnlinkWidget.h"

namespace MyGUI
{

	class _MyGUIExport PointerManager : public UnlinkWidget
	{
		INSTANCE_HEADER(PointerManager);

	public:
		void initialise();
		void shutdown();

	public:

		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void clear();

		void _load(xml::xmlNodePtr _node, const std::string & _file);

		void show();
		void hide();

		inline bool isShow() {return mShow;}

		void setPosition(const IntPoint& _pos);
		void setPointer(const std::string & _name, WidgetPtr _owner);

		void _unlinkWidget(WidgetPtr _widget);

	private:
		std::string mDefaultPointer;
		std::string mTexture;
		IntPoint mPoint;
		bool mShow;

		MapPointerInfo mMapPointers;

		WidgetPtr mWidgetOwner;
		WidgetPtr mMousePointer;


	}; // class PointerManager

} // namespace MyGUI

#endif // __MYGUI_POINTER_MANAGER_H__
