/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/
#ifndef PANEL_H_
#define PANEL_H_

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
		Обновление размеров содержимого.
		Метод updateMeasure вызывается для обновления mDesiredSize по нашему содержимому.
		В этот размер входят наши внешние отступы Margin, внутрение отступы Padding, размер наших рамок и размер самого содержимого.
		При расчете mDesiredSize учитывается MinSize и MaxSize, но только для виджета, без учета внешних отступов Margin.
		В метод передается размер доступной зоны _sizeAvailable в который мы должны себя уместить.
		Размер mDesiredSize не может быть больше чем размер доступной зоны _sizeAvailable.
		*/
		static void updateMeasure(Widget* _widget, const IntSize& _sizeAvailable);

		/*
		вызывается отцом для того чтобы виджет себя физически разместил
		передается _coordPlace это координаты места где виджет будет находиться
		виджет не имеет права заходить за эти координаты,
		в эти координаты не входит внутрение отступы отца Padding, так как они принадлежат отцу
		эти отступы отец уже учел
		*/
		static void updateArrange(Widget* _widget, const IntCoord& _coordPlace);

		static IntSize getDesiredSize(Widget* _widget);

	protected:
		/*
		Запрос размера.
		Метод overrideMeasure вызывается для обновления размера mDesiredSize.
		Метод является перекрываемым, для того чтобы виджеты могли учесть свою внутренюю структуру для размещения содержимого.
		В метод передается доступный размер _sizeAvailable. В этот размер не входят внешние отступы Margin, т.е. это размер для самого виджета.
		В этом методе виджет должен сам учесть свои внутренние отступы Padding, размеры свой рамки и размер содержимого.
		Если размер mDesiredSize будет больше _sizeAvailable то он усечется.
		*/
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable) = 0;

		/*
		Метод overrideArrange для раставления дочерних виджетов.
		Можно использовать размер mDesiredSize дочерних виджетов.
		*/
		virtual void overrideArrange() = 0;

		virtual void onWidgetCreated(Widget* _widget);
		virtual void onWidgetDestroy(Widget* _widget);

	private:
		IntSize mDesiredSize;
	};

} // namespace MyGUI

#endif // PANEL_H_
