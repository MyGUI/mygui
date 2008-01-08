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

	struct AlphaDataInfo
	{
		AlphaDataInfo(WidgetPtr _widget, float _alpha, bool _hide, bool _destroy) :
			widget(_widget), alpha(_alpha), hide(_hide), destroy(_destroy) {}

		WidgetPtr widget;
		float alpha;
		bool hide;
		bool destroy;
	};
	typedef std::vector<AlphaDataInfo> VectorAlphaDataInfo;

	class Tab;
	typedef Tab* TabPtr;

	class _MyGUIExport Tab : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::TabFactory;

	protected:
		Tab(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		void updateBar();

		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender, bool _double);
		void notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender, bool _double);

		int getButtonWidthByName(const Ogre::DisplayString& _text);

		void _showSheet(WidgetPtr _sheet, bool _show, bool _smooth);

		inline void _createSheetButton()
		{
			ButtonPtr button = mWidgetBar->createWidget<Button>(mButtonSkinName, IntCoord(), ALIGN_LEFT | ALIGN_TOP);
			button->eventMouseButtonClick = newDelegate(this, &Tab::notifyPressedBarButtonEvent);
			button->setInternalData((int)mSheetButton.size()); // ���������� �����
			mSheetButton.push_back(button);
		}

		void _addToAlphaController(WidgetPtr _widget, float _alpha, bool _hide = false, bool _enabled = true, bool _destroy = false);
		void _removeFromAlphaController(WidgetPtr _widget);

		void _frameEntered(float _frame);

	public:
		// ��� ������� �������
		inline static const Ogre::String & getType() {static Ogre::String type("Tab"); return type;}

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// ������� ������� ������
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
		// ������ � ���������
		//--------------------------------------------------------------------
		inline size_t getSheetCount()
		{
			return mSheetsInfo.size();
		}

		inline const Ogre::DisplayString& getSheetName(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");
			return mSheetsInfo[_index].name;
		}

		inline int getSheetButtonWidth(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");
			return mSheetsInfo[_index].width;
		}

		inline WidgetPtr getSheet(size_t _index)
		{
			MYGUI_ASSERT(_index < mSheetsInfo.size(), "index out of range");
			return mSheetsInfo[_index].sheet;
		}

		inline WidgetPtr findSheet(const Ogre::DisplayString& _name)
		{
			for (VectorTabSheetInfo::iterator iter=mSheetsInfo.begin(); iter!=mSheetsInfo.end(); ++iter) {
				if ((*iter).name == _name) return (*iter).sheet;
			}
			return null;
		}

		void setSheetName(size_t _index, const Ogre::DisplayString& _name, int _width = DEFAULT);
		void setSheetButtonWidth(size_t _index, int _width = DEFAULT);

		inline WidgetPtr addSheet(const Ogre::DisplayString& _name, int _width = DEFAULT)
		{
			return insertSheet(ITEM_NONE, _name, _width);
		}

		WidgetPtr insertSheet(size_t _index, const Ogre::DisplayString& _name, int _width = DEFAULT);

		void removeSheetIndex(size_t _index);
		void removeSheet(const Ogre::DisplayString& _name);
		void removeSheet(WidgetPtr _sheet);

		void selectSheetIndex(size_t _index, bool _smooth = true);
		void selectSheet(const Ogre::DisplayString& _name, bool _smooth = true);
		void selectSheet(WidgetPtr _sheet, bool _smooth = true);

	private:
		int mOffsetTab; // �������� ���� ��� ������ ������
		bool mButtonShow;
		int mWidthBar; // ������ � ������� ����������� ��� ������
		VectorWidgetPtr mSheetButton; // ������ ������, �� ������ ����� ������ �������
		std::string mButtonSkinName, mEmptySkinName;

		WidgetPtr mWidgetBar;
		ButtonPtr mButtonLeft, mButtonRight, mButtonList;
		VectorWidgetPtr mWidgetsPatch; // ������ �������� ������� ����� �������� ��� ������ ������
		WidgetPtr mEmptyBarWidget;
		WidgetPtr mSheetTemplate;

		// ���������� � ��������
		VectorTabSheetInfo mSheetsInfo;
		size_t mStartIndex;
		size_t mSelectSheet;

		int mButtonDefaultWidth;
		bool mSmoothShow;
		bool mButtonAutoWidth;

		// ������ ��������, ��� ��������� �����
		VectorAlphaDataInfo mVectorAlphaDataInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__

