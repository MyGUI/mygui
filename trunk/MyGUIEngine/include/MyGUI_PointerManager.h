/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Singleton.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_IPointer.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT PointerManager :
		public Singleton<PointerManager>,
		public IUnlinkWidget,
		public MemberObsolete<PointerManager>
	{
	public:
		PointerManager();

		void initialise();
		void shutdown();

		/** Show or hide mouse pointer */
		void setVisible(bool _visible);
		/** Is mouse pointer visible */
		bool isVisible() const;

		/** Set pointer that will be shown
			@param _name of pointer
		*/
		void setPointer(const std::string& _name);
		/** Reset to default pointer */
		void resetToDefaultPointer();

		/** Get default pointer */
		const std::string& getDefaultPointer() const;
		/** Set default pointer */
		void setDefaultPointer(const std::string& _value);

		/** Get layer name where pointer rendered */
		const std::string& getLayerName() const;
		/** Set layer where pointer rendered */
		void setLayerName(const std::string& _value);

		/** Get pointer resource */
		IPointer* getByName(const std::string& _name) const;

		/*events:*/
		/** Event : Mouse pointer has been changed.\n
			signature : void method(const std::string& _pointerName)\n
			@param _pointerName Name of current mouse pointer
		*/
		delegates::CMultiDelegate1<const std::string&>
			eventChangeMousePointer;

	private:
		void _unlinkWidget(Widget* _widget);
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		// создает виджет
		Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

		// удяляет неудачника
		void _destroyChildWidget(Widget* _widget);

		// удаляет всех детей
		void _destroyAllChildWidget();

		void Update();

		void notifyFrameStart(float _time);
		void notifyChangeMouseFocus(Widget* _widget);
		void setPointer(const std::string& _name, Widget* _owner);

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		std::string mDefaultName;
		IntPoint mPoint;
		IntPoint mOldPoint;
		bool mVisible;
		std::string mLayerName;
		std::string mSkinName;

		Widget* mWidgetOwner;
		ImageBox* mMousePointer;
		IPointer* mPointer;
		std::string mCurrentMousePointer;

		bool mIsInitialise;
		std::string mXmlPointerTagName;
		std::string mXmlPropertyTagName;
		std::string mXmlDefaultPointerValue;
	};

} // namespace MyGUI

#endif // __MYGUI_POINTER_MANAGER_H__
