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
#include "MyGUI_WidgetCreator.h"

namespace MyGUI
{

	class _MyGUIExport PointerManager : public UnlinkWidget, public WidgetCreator
	{
		INSTANCE_HEADER(PointerManager);

	public:
		void initialise();
		void shutdown();

	public:

		/** Load additional MyGUI *.pointer file */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		void _load(xml::xmlNodePtr _node, const std::string & _file);

		/** Show pointer*/
		void show();
		/** Hide pointer*/
		void hide();

		/** Return visibility of pointer*/
		inline bool isShow() {return mShow;}

		/** Set pointer position*/
		void setPosition(const IntPoint& _pos);
		/** Set pointer that will be shown
			@param _name of pointer
			@param _owner If _owner widget destroyed - pointer returned to default
		*/
		void setPointer(const std::string & _name, WidgetPtr _owner);
		/** Set default pointer */
		inline void setDefaultPointer() {if (false == mDefaultPointer.empty()) setPointer(mDefaultPointer, null); }

		void _unlinkWidget(WidgetPtr _widget);

		inline const std::string & getDefaultPointer() { return mDefaultPointer; }

	private:

		void clear();
		// создает виджет
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// удяляет неудачника
		virtual void _destroyChildWidget(WidgetPtr _widget);

		// удаляет всех детей
		virtual void _destroyAllChildWidget();

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

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
