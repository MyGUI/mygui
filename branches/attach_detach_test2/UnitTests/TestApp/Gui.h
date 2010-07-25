#pragma once

#include <MyGUI.h>
#include "Widget.h"

namespace MyGUI
{
	class Gui2 :
		public Singleton<Gui2>,
		public ControlContainer
	{
	public:
		Gui2();
		~Gui2();

		Control* createChild();
		void destroyChild(Control* _widget);
		void destroyAllChilds();

		size_t getChildCount();
		Control* getChild(size_t _index);

		virtual void detachControl(Control* _widget);
		virtual void attachControl(Control* _widget);

	private:
		void addChild(Control* _widget);
		void removeChild(Control* _widget);

	private:
		VectorControlPtr mChilds;
	};
}