/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/
#ifndef __PANEL_H__
#define __PANEL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class Panel :
		public Widget
	{
		MYGUI_RTTI_DERIVED( Panel )

	public:
		Panel();

		/** Set widget position (position of left top corner) */
		virtual void setPosition(const IntPoint& _value);
		/** Set widget size */
		virtual void setSize(const IntSize& _value);
		/** Set widget position and size */
		virtual void setCoord(const IntCoord& _value);

		//static void invalidateMeasure(Widget* _widget);
		/*
		���������� �������� �����������.
		����� updateMeasure ���������� ��� ���������� mDesiredSize �� ������ �����������.
		� ���� ������ ������ ���� ������� ������� Margin, ��������� ������� Padding, ������ ����� ����� � ������ ������ �����������.
		��� ������� mDesiredSize ����������� MinSize � MaxSize, �� ������ ��� �������, ��� ����� ������� �������� Margin.
		� ����� ���������� ������ ��������� ���� _sizeAvailable � ������� �� ������ ���� ��������.
		������ mDesiredSize �� ����� ���� ������ ��� ������ ��������� ���� _sizeAvailable.
		*/
		static void updateMeasure(Widget* _widget, const IntSize& _sizeAvailable);

		/*
		���������� ����� ��� ���� ����� ������ ���� ��������� ���������
		���������� _coordPlace ��� ���������� ����� ��� ������ ����� ����������
		������ �� ����� ����� �������� �� ��� ����������,
		� ��� ���������� �� ������ ��������� ������� ���� Padding, ��� ��� ��� ����������� ����
		��� ������� ���� ��� ����
		*/
		static void updateArrange(Widget* _widget, const IntCoord& _coordPlace);

		static IntSize getDesiredSize(Widget* _widget);

	protected:
		/*
		������ �������.
		����� overrideMeasure ���������� ��� ���������� ������� mDesiredSize.
		����� �������� �������������, ��� ���� ����� ������� ����� ������ ���� ��������� ��������� ��� ���������� �����������.
		� ����� ���������� ��������� ������ _sizeAvailable. � ���� ������ �� ������ ������� ������� Margin, �.�. ��� ������ ��� ������ �������.
		� ���� ������ ������ ������ ��� ������ ���� ���������� ������� Padding, ������� ���� ����� � ������ �����������.
		���� ������ mDesiredSize ����� ������ _sizeAvailable �� �� ��������.
		*/
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable) = 0;

		/*
		����� overrideArrange ��� ����������� �������� ��������.
		����� ������������ ������ mDesiredSize �������� ��������.
		*/
		virtual void overrideArrange() = 0;

		virtual void onWidgetCreated(Widget* _widget);
		virtual void onWidgetDestroy(Widget* _widget);

	private:
		IntSize mDesiredSize;
	};

} // namespace MyGUI

#endif // __PANEL_H__
