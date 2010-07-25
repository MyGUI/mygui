#pragma once

#include <MyGUI.h>

namespace MyGUI
{
	class Control;

	class Skinable
	{
	public:
		Skinable();
		virtual ~Skinable();

	protected:
		void onVisualChildAdded(Control* _child);
		void onVisualChildRemoved(Control* _child);

		void onDestroySkin();
		void onCreateSkin(ResourceSkin* _info);
	};
}