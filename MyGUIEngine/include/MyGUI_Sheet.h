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
		Sheet(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Sheet();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Set sheet caption */
		void setCaption(const Ogre::UTFString & _caption);
		/** Get sheet caption */
		const Ogre::UTFString & getCaption();

		//--------------------------------------------------------------------
		// дубликаты методов у таба
		//--------------------------------------------------------------------
		/** Get sheet name */
		const Ogre::UTFString & getSheetName();
		/** Set sheet name and optionally width */
		void setSheetName(const Ogre::UTFString & _name, int _width = DEFAULT);

		/** Get width of sheet button */
		int getSheetButtonWidth();
		/** Set width of sheet button */
		void setSheetButtonWidth(int _width = DEFAULT);

		/** Select sheet (show) */
		void selectSheet(bool _smooth = true);

		/** Remove sheet */
		void removeSheet();

	private:
		TabPtr mOwner;

	}; // class _MyGUIExport Sheet : public Widget

} // namespace MyGUI

#endif // __MYGUI_SHEET_H__
