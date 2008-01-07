/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_TAB_H__
#define __MYGUI_TAB_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TabFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	struct TabSheetInfo
	{
		TabSheetInfo(int _width, const Ogre::DisplayString& _name, WidgetPtr _sheet) :
			width(_width), name(_name), sheet(_sheet)
		{
		}

		int width;
		Ogre::DisplayString name;
		WidgetPtr sheet;
	};

	typedef std::vector<TabSheetInfo> VectorTabSheetInfo;

	class Tab;
	typedef Tab* TabPtr;

	class _MyGUIExport Tab : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::TabFactory;

	protected:
		Tab(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		void updateBar();

		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double);
		void notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender, bool _double);

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("Tab"); return type;}

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// сделать видимой кнопку
		void showBarButton(size_t _index);
		void showBarButton(const Ogre::DisplayString& _name);
		inline void showBarSelectButton() {showBarButton(mSelectSheet);}

	private:
		int mOffsetTab; // смещение бара при показе кнопок
		bool mButtonShow;
		int mWidthBar; // ширина в которую помещаються все кнопки
		VectorWidgetPtr mSheetButton; // список кнопок, не должно равно списку страниц
		std::string mButtonSkinName, mEmptySkinName;

		WidgetPtr mWidgetBar;
		ButtonPtr mButtonLeft, mButtonRight, mButtonList;
		VectorWidgetPtr mWidgetsPatch; // список виджетов которые нужно показать при показе кнопки
		WidgetPtr mEmptyBarWidget;

		// информация о вкладках
		VectorTabSheetInfo mSheetsInfo;
		size_t mStartIndex;
		size_t mSelectSheet;
	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__

