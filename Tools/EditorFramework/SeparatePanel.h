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
	class SeparatePanel : public Control
	{
	public:
		~SeparatePanel() override;

		void setPanelAlign(MyGUI::Align _value);
		MyGUI::Align getPanelAlign() const;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyChangeCoord(MyGUI::Widget* _sender);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

		void invalidateSize(
			const MyGUI::IntCoord& _firstPanel,
			const MyGUI::IntCoord& _separatorH,
			const MyGUI::IntCoord& _separatorV,
			const MyGUI::IntCoord& _secondPanel);
		void updateSize(int& _firstWidth, int& _secondWidth, int _separatorWidth, int _mainWidth, int _defaultSize)
			const;
		void moveSeparator(const MyGUI::IntPoint& _mousePosition);
		void loadDefaultSize();
		void saveDefaultSize();

	private:
		MyGUI::Widget* mFirstPanel{};
		MyGUI::Widget* mSecondPanel{};
		MyGUI::Widget* mSeparatorH{};
		MyGUI::Widget* mSeparatorV{};
		int mMinSize{};
		MyGUI::Align mPanelAlign{MyGUI::Align::Left};
		MyGUI::IntSize mDefaultPanelSize;
		MyGUI::IntPoint mMousePressedOffset;
		std::string mSaveAs;
	};

}

#endif
