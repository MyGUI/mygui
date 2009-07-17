/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_POINTER_MANAGER_H__
#define __MYGUI_POINTER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_IWidgetCreator.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_IPointer.h"

namespace MyGUI
{

	class MYGUI_EXPORT PointerManager :
		public IUnlinkWidget,
		public IWidgetCreator
	{
		MYGUI_INSTANCE_HEADER(PointerManager);

	public:
		void initialise();
		void shutdown();

	public:
		/** Load additional MyGUI *_pointer.xml file */
		bool load(const std::string& _file);

		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		/** Show or hide mouse pointer */
		void setVisible(bool _visible);
		/** Is mouse pointer visible */
		bool isVisible() { return mVisible; }

		/** Set pointer position */
		void setPosition(const IntPoint& _pos);
		/** Set pointer that will be shown
			@param _name of pointer
			@param _owner If _owner widget destroyed - pointer returned to default
		*/
		void setPointer(const std::string& _name, WidgetPtr _owner);
		/** Set default pointer */
		void setDefaultPointer();

		/** Get default pointer */
		const std::string& getDefaultPointer() { return mDefaultPointer; }
		void setDeafultPointer(const std::string& _value);

		const std::string& getLayerName() { return mLayerName; }
		void setLayerName(const std::string& _value);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void PointerManager::setVisible(bool _visible)")
		void show() { setVisible(true); }
		MYGUI_OBSOLETE("use : void PointerManager::setVisible(bool _visible)")
		void hide() { setVisible(false); }
		MYGUI_OBSOLETE("use : bool PointerManager::isVisible()")
		bool isShow() { return isVisible(); }

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void _unlinkWidget(WidgetPtr _widget);

		// создает виджет
		virtual WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

		// удяляет неудачника
		virtual void _destroyChildWidget(WidgetPtr _widget);

		// удаляет всех детей
		virtual void _destroyAllChildWidget();

		void Update();

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		std::string mDefaultPointer;
		IntPoint mPoint;
		bool mVisible;
		std::string mLayerName;

		WidgetPtr mWidgetOwner;
		StaticImagePtr mMousePointer;
		IPointer* mPointer;
	};

} // namespace MyGUI

#endif // __MYGUI_POINTER_MANAGER_H__
