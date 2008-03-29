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

	protected:
		Sheet(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Sheet();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		void setCaption(const Ogre::UTFString & _caption);
		const Ogre::UTFString & getCaption();

		//--------------------------------------------------------------------
		// дубликаты методов у таба
		//--------------------------------------------------------------------
		const Ogre::UTFString & getSheetName();
		void setSheetName(const Ogre::UTFString & _name, int _width = DEFAULT);

		int getSheetButtonWidth();
		void setSheetButtonWidth(int _width = DEFAULT);

		void selectSheet(bool _smooth = true);

		void removeSheet();

	private:
		TabPtr mOwner;

	}; // class _MyGUIExport Sheet : public Widget

} // namespace MyGUI

#endif // __MYGUI_SHEET_H__
