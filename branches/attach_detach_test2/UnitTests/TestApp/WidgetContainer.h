#pragma once

#include <vector>

namespace MyGUI
{
	class Control;
	typedef std::vector<Control*> VectorControlPtr;

	class ControlContainer
	{
	public:
		virtual ~ControlContainer() { }

		virtual void detachControl(Control* _widget) = 0;
		virtual void attachControl(Control* _widget) = 0;
	};
}