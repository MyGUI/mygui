#include "WorkspaceControl.h"
#include "WidgetCreatorManager.h"

namespace tools
{
	WorkspaceControl::WorkspaceControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("WorkspaceControl.layout", _parent)
	{
		mMainWidget->eventMouseMove += newDelegate(this, &WorkspaceControl::notifyMouseMouseMove);
		mMainWidget->eventMouseDrag += newDelegate(this, &WorkspaceControl::notifyMouseMouseDrag);
		mMainWidget->eventMouseButtonPressed += newDelegate(this, &WorkspaceControl::notifyMouseButtonPressed);
		mMainWidget->eventMouseButtonReleased += newDelegate(this, &WorkspaceControl::notifyMouseButtonReleased);
	}

	WorkspaceControl::~WorkspaceControl()
	{
		mMainWidget->eventMouseMove -= newDelegate(this, &WorkspaceControl::notifyMouseMouseMove);
		mMainWidget->eventMouseDrag -= newDelegate(this, &WorkspaceControl::notifyMouseMouseDrag);
		mMainWidget->eventMouseButtonPressed -= newDelegate(this, &WorkspaceControl::notifyMouseButtonPressed);
		mMainWidget->eventMouseButtonReleased -= newDelegate(this, &WorkspaceControl::notifyMouseButtonReleased);
	}

	void WorkspaceControl::notifyMouseMouseMove(MyGUI::Widget* _sender, int _left, int _top)
	{
		WidgetCreatorManager::getInstance().notifyMouseMouseMove(MyGUI::IntPoint(_left, _top));
	}

	void WorkspaceControl::notifyMouseMouseDrag(MyGUI::Widget* _sender, int _left, int _top)
	{
		WidgetCreatorManager::getInstance().notifyMouseMouseDrag(MyGUI::IntPoint(_left, _top));
	}

	void WorkspaceControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
			WidgetCreatorManager::getInstance().notifyMouseButtonPressed(MyGUI::IntPoint(_left, _top));
	}

	void WorkspaceControl::notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
			WidgetCreatorManager::getInstance().notifyMouseButtonReleased(MyGUI::IntPoint(_left, _top));
	}

} // namespace tools
