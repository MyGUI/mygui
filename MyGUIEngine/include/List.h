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

		// ��������� ������� ������, ����� ��� ������������
		void changeIndex();

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// ���� ���� � �������
		WidgetChild mWidgetLines;

		int mHeightLine; // ������ ����� ������
//		int mCountLine; // ����������� �����
		int mTopIndex; // ������ ������ �������� ��������
		int mOffsetTop; // ������� ��������
		int mRangeIndex; // ����������� ������

		std::vector<Ogre::DisplayString> mStringArray;

	}; // class List : public Widget

	typedef List * ListPtr;

} // namespace MyGUI

#endif // _LIST_H_