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

		Widget* createWidget(const std::string& _skin);
		void destroyChild(Widget* _widget);
		void destroyAllChilds();

		size_t getChildCount();
		Widget* getChild(size_t _index);

	private:
		VectorWidgetPtr mChilds;
	};
}