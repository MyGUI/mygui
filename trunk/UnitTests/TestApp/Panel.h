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
		void updateArrange(const IntCoord& _coordPlace, const IntSize& _oldsize);

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
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);

		/*
		����� overrideArrange ��� ����������� �������� ��������.
		����� ������������ ������ mDesiredSize �������� ��������.
		*/
		virtual void overrideArrange(const IntSize& _sizeOld);

	private:
		IntSize mDesiredSize;
	};

} // namespace MyGUI

#endif // __PANEL_H__
