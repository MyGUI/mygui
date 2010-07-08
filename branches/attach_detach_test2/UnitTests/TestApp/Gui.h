#pragma once

#include <MyGUI.h>
#include "Widget.h"

namespace demo
{
	class Gui :
		public MyGUI::Singleton<Gui>
	{
	public:
		Gui();
		~Gui();

		Widget* createChild();
		void destroyChild(Widget* _widget);
		void destroyAllChilds();

		size_t getChildCount();
		Widget* getChild(size_t _index);

		void detachWidget(Widget* _widget);
		void attachWidget(Widget* _widget);

	private:
		void addChild(Widget* _widget);
		void removeChild(Widget* _widget);

	private:
		VectorWidgetPtr mChilds;
	};
}