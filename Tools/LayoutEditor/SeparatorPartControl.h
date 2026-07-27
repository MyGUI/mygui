#ifndef _2b447a49_563a_4768_84f6_f7b835381a82_
#define _2b447a49_563a_4768_84f6_f7b835381a82_

#include "Control.h"

namespace tools
{

	class SeparatorPartControl : public Control
	{
	public:
		~SeparatorPartControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

	private:
		MyGUI::Widget* mLeftPanel{nullptr};
		MyGUI::Widget* mRightPanel{nullptr};
		MyGUI::Widget* mSeparatorH{nullptr};

		int mMinSizeLeft{0};
		int mMinSizeRight{0};

		MyGUI::IntCoord mStartLeftPanel;
		MyGUI::IntCoord mStartRightPanel;
		MyGUI::IntCoord mStartSeparatorH;

		MyGUI::IntPoint mStartMousePosition;
	};

}

#endif
