#ifndef __SEPARATOR_PART_CONTROL_H__
#define __SEPARATOR_PART_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class SeparatorPartControl :
		public wraps::BaseLayout
	{
	public:
		SeparatorPartControl(const std::string& _layout, MyGUI::Widget* _parent);
		virtual ~SeparatorPartControl();

	private:
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

		void notifyFrameStart(float _time);

	private:
		MyGUI::Widget* mLeftPanel;
		MyGUI::Widget* mRightPanel;
		MyGUI::Widget* mSeparatorH;

		int mMinSizeLeft;
		int mMinSizeRight;

		MyGUI::IntCoord mStartLeftPanel;
		MyGUI::IntCoord mStartRightPanel;
		MyGUI::IntCoord mStartSeparatorH;

		MyGUI::IntPoint mStartMousePosition;
		bool mChange;
	};

} // namespace tools

#endif // __SEPARATOR_PART_CONTROL_H__
