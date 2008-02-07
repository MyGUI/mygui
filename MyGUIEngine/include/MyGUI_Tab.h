/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_TAB_H__
#define __MYGUI_TAB_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	typedef std::vector<SheetPtr> VectorSheetPtr;

	struct TabSheetInfo
	{
		TabSheetInfo(int _width, const Ogre::DisplayString& _name, SheetPtr _sheet) :
			width(_width), name(_name), sheet(_sheet)
		{
		}

		int width;
		Ogre::DisplayString name;
		SheetPtr sheet;
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

		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender);
		void notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender);

		int getButtonWidthByName(const Ogre::DisplayString& _text);

		void _showSheet(SheetPtr _sheet, bool _show, bool _smooth);

		void _createSheetButton();

	public:
		// тип данного виджета
		inline static const Ogre::String & _getType() {static Ogre::String type("Tab"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// переопределяем для особого обслуживания страниц
		virtual WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _name = "");

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// сделать видимой кнопку
		void showBarButton(size_t _index);
		void showBarButton(const Ogre::DisplayString& _name);
		inline void showBarSelectButton() {showBarButton(mSelectSheet);}

		inline void setButtonDefaultWidth(int _width)
		{
			mButtonDefaultWidth = _width;
			if (mButtonDefaultWidth < 1) mButtonDefaultWidth = 1;
			mButtonAutoWidth = false;
		}
		inline int getButtonDefaultWidth()
		{
			return mButtonDefaultWidth;
		}

		inline void setButtonAutoWidth(bool _auto)
		{
			mButtonAutoWidth = _auto;
		}
		inline bool getButtonAutoWidth()
		{
			return mButtonAutoWidth;
		}

		inline void setSmoothShow(bool _smooth)
		{
			mSmoothShow = _smooth;
		}
		inline bool getSmoothShow()
		{
			return mSmoothShow;
		}

		//--------------------------------------------------------------------
		// работа с вкладками
		//--------------------------------------------------------------------
		inline size_t getSheetCount()
		{
			return mSheetsInfo.size();
		}

		inline const Ogre::DisplayString& getSheetName(SheetPtr _sheet)
		{
			for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
				if (mSheetsInfo[pos].sheet == _sheet) return mSheetsInfo[pos].name;
			}
			MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
		}

		inline const Ogre::DisplayString& getSheetNameIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "getSheetNameIndex: index " << _index <<" out of range");
			return mSheetsInfo[_index].name;
		}

		inline int getSheetButtonWidthIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "getSheetButtonWidthIndex: index " << _index <<" out of range");
			return mSheetsInfo[_index].width;
		}
		inline int getSheetButtonWidth(SheetPtr _sheet)
		{
			for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
				if (mSheetsInfo[pos].sheet == _sheet) mSheetsInfo[pos].width;
			}
			MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
		}

		inline SheetPtr getSheet(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "getSheet: index " << _index <<" out of range");
			return mSheetsInfo[_index].sheet;
		}

		inline SheetPtr findSheet(const Ogre::DisplayString& _name)
		{
			for (VectorTabSheetInfo::iterator iter=mSheetsInfo.begin(); iter!=mSheetsInfo.end(); ++iter) {
				if ((*iter).name == _name) return (*iter).sheet;
			}
			return null;
		}

		void setSheetNameIndex(size_t _index, const Ogre::DisplayString& _name, int _width = DEFAULT);
		void setSheetName(SheetPtr _sheet, const Ogre::DisplayString& _name, int _width = DEFAULT);

		void setSheetButtonWidthIndex(size_t _index, int _width = DEFAULT);
		void setSheetButtonWidth(SheetPtr _sheet, int _width = DEFAULT);

		SheetPtr addSheet(const Ogre::DisplayString& _name, int _width = DEFAULT);
		SheetPtr insertSheet(size_t _index, const Ogre::DisplayString& _name, int _width = DEFAULT);

		void removeSheetIndex(size_t _index);
		void removeSheet(const Ogre::DisplayString& _name);
		void removeSheet(SheetPtr _sheet);

		void selectSheetIndex(size_t _index, bool _smooth = true);
		void selectSheet(const Ogre::DisplayString& _name, bool _smooth = true);
		void selectSheet(SheetPtr _sheet, bool _smooth = true);
		inline size_t getSelectSheetIndex() {return mSelectSheet;}

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
		WidgetPtr mSheetTemplate;

		// информация о вкладках
		VectorTabSheetInfo mSheetsInfo;
		size_t mStartIndex;
		size_t mSelectSheet;

		int mButtonDefaultWidth;
		bool mSmoothShow;
		bool mButtonAutoWidth;

	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__

