/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_SHEET_H__
#define __MYGUI_SHEET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport Sheet : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::SheetFactory;
		friend class Tab;

	protected:
		Sheet(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		// перекрываем методы, но через базовый они будут доступны
		void setPosition(const IntPoint& _pos);
		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		void show();
		void hide();

	public:
		// тип данного виджета
		inline static const Ogre::String & _getType() {static Ogre::String type("Sheet"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void setCaption(const Ogre::DisplayString & _caption);
		const Ogre::DisplayString & getCaption();

		//--------------------------------------------------------------------
		// дубликаты методов у таба
		//--------------------------------------------------------------------
		const Ogre::DisplayString & getSheetName();
		void setSheetName(const Ogre::DisplayString & _name, int _width = DEFAULT);

		int getSheetButtonWidth();
		void setSheetButtonWidth(int _width = DEFAULT);

		void selectSheet(bool _smooth = true);

		void removeSheet();

	private:
		TabPtr mOwner;

	}; // class _MyGUIExport Sheet : public Widget

} // namespace MyGUI

#endif // __MYGUI_SHEET_H__
