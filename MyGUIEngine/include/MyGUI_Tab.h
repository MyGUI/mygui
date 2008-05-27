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
		TabSheetInfo(int _width, const Ogre::UTFString& _name, SheetPtr _sheet) :
			width(_width), name(_name), sheet(_sheet)
		{
		}

		int width;
		Ogre::UTFString name;
		SheetPtr sheet;
	};

	typedef std::vector<TabSheetInfo> VectorTabSheetInfo;

	class _MyGUIExport Tab : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::TabFactory;
		// для уведобления об удалении
		friend class Sheet;

	protected:
		Tab(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Tab();

		void updateBar();

		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender);
		void notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender);

		int getButtonWidthByName(const Ogre::UTFString& _text);

		void _showSheet(SheetPtr _sheet, bool _show, bool _smooth);

		void _createSheetButton();

		// вкладка при удалении уведомляет таб
		void _notifyDeleteSheet(SheetPtr _sheet);

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// переопределяем для особого обслуживания страниц
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// сделать видимой кнопку
		/** Show bar button
			@param _index of button
		*/
		void showBarButton(size_t _index);
		/** Show bar button
			@param _name of button
		*/
		void showBarButton(const Ogre::UTFString& _name);
		/** Show selected bar button */
		inline void showBarSelectButton() {showBarButton(mSelectSheet);}

		/** Set default button width and disable autowidth mode */
		void setButtonDefaultWidth(int _width);

		/** Get default button width */
		inline int getButtonDefaultWidth()
		{
			return mButtonDefaultWidth;
		}

		/** Enable or disable button auto width */
		void setButtonAutoWidth(bool _auto);
		/** Get button auto width flag */
		inline bool getButtonAutoWidth()
		{
			return mButtonAutoWidth;
		}

		/** Enable or disable smooth sheets showing*/
		inline void setSmoothShow(bool _smooth)
		{
			mSmoothShow = _smooth;
		}
		/** Get smooth sheets showing flag */
		inline bool getSmoothShow()
		{
			return mSmoothShow;
		}

		//--------------------------------------------------------------------
		// работа с вкладками
		//--------------------------------------------------------------------
		/** Get sheets count */
		inline size_t getSheetCount()
		{
			return mSheetsInfo.size();
		}

		/** Get _sheet name */
		inline const Ogre::UTFString& getSheetName(SheetPtr _sheet)
		{
			for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
				if (mSheetsInfo[pos].sheet == _sheet) return mSheetsInfo[pos].name;
			}
			MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
		}

		/** Get sheet name by _index */
		inline const Ogre::UTFString& getSheetNameIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "getSheetNameIndex: index " << (unsigned int)_index <<" out of range");
			return mSheetsInfo[_index].name;
		}

		/** Get sheet button width by _index */
		inline int getSheetButtonWidthIndex(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "getSheetButtonWidthIndex: index " << (unsigned int)_index <<" out of range");
			return mSheetsInfo[_index].width;
		}
		/** Get _sheet button width */
		inline int getSheetButtonWidth(SheetPtr _sheet)
		{
			for (size_t pos=0; pos<mSheetsInfo.size(); pos++) {
				if (mSheetsInfo[pos].sheet == _sheet) mSheetsInfo[pos].width;
			}
			MYGUI_EXCEPT("sheet (" << _sheet << ") not found");
		}

		/** Get sheet pointer by _index */
		inline SheetPtr getSheet(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "getSheet: index " << (unsigned int)_index <<" out of range");
			return mSheetsInfo[_index].sheet;
		}

		/** Get sheet pointer by _name */
		inline SheetPtr findSheet(const Ogre::UTFString& _name)
		{
			for (VectorTabSheetInfo::iterator iter=mSheetsInfo.begin(); iter!=mSheetsInfo.end(); ++iter) {
				if ((*iter).name == _name) return (*iter).sheet;
			}
			return null;
		}

		/** Set sheet name
			@param _index of sheet
			@param _width new width
		*/
		void setSheetNameIndex(size_t _index, const Ogre::UTFString& _name, int _width = DEFAULT);
		/** Set _sheet name
			@param _width new width
		*/
		void setSheetName(SheetPtr _sheet, const Ogre::UTFString& _name, int _width = DEFAULT);

		/** Set sheet button width
			@param _index of sheet
			@param _width new width
		*/
		void setSheetButtonWidthIndex(size_t _index, int _width = DEFAULT);
		/** Set _sheet button width 
			@param _width new width
		*/
		void setSheetButtonWidth(SheetPtr _sheet, int _width = DEFAULT);

		/** Add sheet at the end
			@param _name of new sheet
			@param _width of new sheet
		*/
		SheetPtr addSheet(const Ogre::UTFString& _name, int _width = DEFAULT);
		/** Insert sheet at specified position
			@param _index where new sheet wil be
			@param _name of new sheet
			@param _width of sheet
		*/
		SheetPtr insertSheet(size_t _index, const Ogre::UTFString& _name, int _width = DEFAULT);

		/** Removee sheet by _index */
		void removeSheetIndex(size_t _index);
		/** Removee sheet by _name */
		void removeSheet(const Ogre::UTFString& _name);
		/** Removee sheet by _sheet pointer */
		void removeSheet(SheetPtr _sheet);

		/** Select sheet by _index, with _smooth selecting */
		void selectSheetIndex(size_t _index, bool _smooth = true);
		/** Select sheet by _name, with _smooth selecting */
		void selectSheet(const Ogre::UTFString& _name, bool _smooth = true);
		/** Select sheet by _sheet pointer, with _smooth selecting */
		void selectSheet(SheetPtr _sheet, bool _smooth = true);
		/** Get selected sheet index */
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

		// флаг, чтобы отсеч уведомления от вкладок, при общем шутдауне виджета
		bool mShutDown;

	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__

