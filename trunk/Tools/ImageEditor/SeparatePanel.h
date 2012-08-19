/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _4eddcc54_41d1_4881_8840_89c6f0894589_
#define _4eddcc54_41d1_4881_8840_89c6f0894589_

#include "Control.h"

namespace tools
{
	class SeparatePanel :
		public Control
	{
	public:
		SeparatePanel();
		virtual ~SeparatePanel();

		void setPanelAlign(MyGUI::Align _value);
		MyGUI::Align getPanelAlign() const;

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyChangeCoord(MyGUI::Widget* _sender);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

		void invalidateSize(const MyGUI::IntCoord& _firstPanel, const MyGUI::IntCoord& _separatorH, const MyGUI::IntCoord& _separatorV, const MyGUI::IntCoord& _secondPanel);
		void updateSize(int& _firstCoord, int& _secondCoord, int _separatorWidth, int _mainSize, int _defaultSize);
		void moveSeparator(const MyGUI::IntPoint& _mousePosition);
		void loadDefaultSize();
		void saveDefaultSize();

	private:
		MyGUI::Widget* mFirstPanel;
		MyGUI::Widget* mSecondPanel;
		MyGUI::Widget* mSeparatorH;
		MyGUI::Widget* mSeparatorV;
		int mMinSize;
		MyGUI::Align mPanelAlign;
		MyGUI::IntSize mDefaultPanelSize;
		MyGUI::IntPoint mMousePressedOffset;
		std::string mSaveAs;
	};

}

#endif
