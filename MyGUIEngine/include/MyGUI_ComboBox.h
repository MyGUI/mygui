/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_H__
#define __MYGUI_COMBO_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_Edit.h"

namespace MyGUI
{

	class ComboBox;
	typedef ComboBox* ComboBoxPtr;

	class _MyGUIExport ComboBox : public Edit
	{
		// для вызова закрытого конструктора
		friend factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("ComboBox"); return type;}

	private:
		WidgetPtr mButton;


	}; // class _MyGUIExport StaticText : public Widget

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__