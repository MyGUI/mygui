#ifndef _LIST_H_
#define _LIST_H_

#include "Widget.h"
#include "VScroll.h"

namespace MyGUI
{

	class _MyGUIExport List : public Widget
	{
		// ��� ������ ��������� ������������
		friend WidgetFactory<List>;

	protected:
		List(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("List"); return type;};


	protected:
		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel);

		virtual void size(int _cx, int _cy);
		virtual void move(int _x, int _y, int _cx, int _cy);

		void updateScroll();
		void updateLine();

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// ���� ���� � �������
		WidgetChild mWidgetLines;

		size_t mHeightLine; // ������ ����� ������
		size_t mCountLine; // ����������� �����
		size_t mTopIndex; // ������ ������ �������� ��������
		int mOffsetTop; // ������� ��������
		size_t mRangeIndex; // ����������� ������

	}; // class List : public Widget

	typedef List * ListPtr;

} // namespace MyGUI

#endif // _LIST_H_