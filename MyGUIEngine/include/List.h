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

		inline size_t getIndexCount() {return mStringArray.size();}
		inline const Ogre::DisplayString & getIndexString(size_t _index) {assert(_index < mStringArray.size()); return mStringArray[_index];}
		inline void setIndexString(size_t _index, const Ogre::DisplayString & _item) {assert(_index < mStringArray.size()); mStringArray[_index]=_item; redrawIndex(_index);}
		void insertIndexString(size_t _index, const Ogre::DisplayString & _item);
		inline void addIndexString(const Ogre::DisplayString & _item) {insertIndexString((size_t)-1, _item);}
		void deleteIndexString(size_t _index);


	protected:
		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel);

		virtual void size(int _cx, int _cy);
		virtual void move(int _x, int _y, int _cx, int _cy);

		void updateScroll();
		void updateLine(bool _reset = false);

		// ��������� ������� ������, ����� ��� ������������
		void changeIndex(size_t _start=0);

		// �������������� ������
		void redrawIndex(size_t _index);

		// ������� ������ �� ������
		void _deleteString(size_t _index);

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// ���� ���� � �������
		WidgetChild mWidgetLines;

		int mHeightLine; // ������ ����� ������
		int mTopIndex; // ������ ������ �������� ��������
		int mOffsetTop; // ������� ��������
		int mRangeIndex; // ����������� ������
		size_t mLastRedrawLine; // ��������� �������������� �����

		std::vector<Ogre::DisplayString> mStringArray;

	}; // class List : public Widget

	typedef List * ListPtr;

} // namespace MyGUI

#endif // _LIST_H_