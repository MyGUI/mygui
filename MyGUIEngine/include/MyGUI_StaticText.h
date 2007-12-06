/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_STATIC_TEXT_H__
#define __MYGUI_STATIC_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_StaticTextFactory.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class StaticText;
	typedef StaticText* StaticTextPtr;

	class _MyGUIExport StaticText : public Widget
	{
		// ��� ������ ��������� ������������
		friend factory::StaticTextFactory;

	protected:
		StaticText(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:
		// ��� ������� �������
		inline static const Ogre::String & getType() {static Ogre::String type("StaticText"); return type;}


	}; // class _MyGUIExport StaticText : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_TEXT_H__