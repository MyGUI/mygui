#include "Precompiled.h"
#include "WorkspaceControl.h"
#include "SettingsManager.h"
//#include "WidgetCreatorManager.h"

namespace tools
{
	WorkspaceControl::WorkspaceControl(MyGUI::Widget* _parent) :
		TextureToolControl(_parent)
	{
		//mMainWidget->eventMouseMove += newDelegate(this, &WorkspaceControl::notifyMouseMouseMove);
		//mMainWidget->eventMouseDrag += newDelegate(this, &WorkspaceControl::notifyMouseMouseDrag);
		//mMainWidget->eventMouseButtonPressed += newDelegate(this, &WorkspaceControl::notifyMouseButtonPressed);
		//mMainWidget->eventMouseButtonReleased += newDelegate(this, &WorkspaceControl::notifyMouseButtonReleased);
		SettingsSector* sector = SettingsManager::getInstance().getSector("Workspace");
		MyGUI::IntSize size = sector->getPropertyValue<MyGUI::IntSize>("TextureSize");
		setTextureName(sector->getPropertyValue("TextureName"));
		setTextureRegion(MyGUI::IntCoord(0, 0, size.width, size.height));
	}

	WorkspaceControl::~WorkspaceControl()
	{
		//mMainWidget->eventMouseMove -= newDelegate(this, &WorkspaceControl::notifyMouseMouseMove);
		//mMainWidget->eventMouseDrag -= newDelegate(this, &WorkspaceControl::notifyMouseMouseDrag);
		//mMainWidget->eventMouseButtonPressed -= newDelegate(this, &WorkspaceControl::notifyMouseButtonPressed);
		//mMainWidget->eventMouseButtonReleased -= newDelegate(this, &WorkspaceControl::notifyMouseButtonReleased);
	}

	/*void WorkspaceControl::notifyMouseMouseMove(MyGUI::Widget* _sender, int _left, int _top)
	{
		WidgetCreatorManager::getInstance().notifyMouseMouseMove(MyGUI::IntPoint(_left, _top));
	}

	void WorkspaceControl::notifyMouseMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
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
	}*/

} // namespace tools
