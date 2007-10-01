#ifndef _SCROLLBASE_H_
#define _SCROLLBASE_H_

#include "Widget.h"
#include "delegate.h"

namespace MyGUI
{

	class _MyGUIExport ScrollBase : public Widget
	{

	protected:
		ScrollBase(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMove(MyGUI::WidgetPtr _sender, int _x, int _y);

		virtual void notifyTrackMove(int _x, int _y) = 0;

		void setScrollRange(size_t _range);
		inline size_t getScrollRange() {return mScrollRange;}

		void setScrollPosition(size_t _position);
		inline size_t getScrollPosition() {return mScrollPosition;}

		inline void setScrollPage(size_t _page = 1) {mScrollPage = _page;}
		inline size_t getScrollPage() {return mScrollPage;}

		virtual void updateTrack() = 0;
		virtual void size(int _cx, int _cy);
		virtual void move(int _x, int _y, int _cx, int _cy);

		// изменилось положение скрола
		EventSimpleDataInfo eventScrollChangePosition;

	protected:
		// наши кнопки
		WidgetPtr mWidgetStart;
		WidgetPtr mWidgetEnd;
		WidgetPtr mWidgetTrack;

		// размеры окна перед началом его изменений
		IntRect m_preActionRect;

		// диапазон на который трек может двигаться
		size_t mSkinRangeStart;
		size_t mSkinRangeEnd;

		size_t mScrollRange;
		size_t mScrollPosition;
		size_t mScrollPage; // на сколько перещелкивать, при щелчке на кнопке

	}; // class ScrollBase : public Widget

	typedef ScrollBase * ScrollBasePtr;

} // namespace MyGUI


#endif // _SCROLLBASE_H_