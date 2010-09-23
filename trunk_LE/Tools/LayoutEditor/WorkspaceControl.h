#ifndef __WORKSPACE_CONTROL_H__
#define __WORKSPACE_CONTROL_H__

#include "TextureToolControl.h"
#include "PropertiesPanelView.h"
#include "WidgetsWindow.h"

namespace tools
{
	class WorkspaceControl :
		public TextureToolControl
	{
	public:
		WorkspaceControl(MyGUI::Widget* _parent = nullptr);
		virtual ~WorkspaceControl();

	/*private:
		void notifyMouseMouseMove(MyGUI::Widget* _sender, int _left, int _top);
		void notifyMouseMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);*/
	};

} // namespace tools

#endif // __WORKSPACE_CONTROL_H__
