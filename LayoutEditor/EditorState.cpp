
#include "BasisManager.h"
#include "EditorState.h"
#include "WidgetContainer.h"

#include "MyGUI.h"

#define ASSIGN_FUNCTION(x,y) MyGUI::WidgetManager::getInstance().findWidgetT(x)->eventMouseButtonClick = MyGUI::newDelegate(this, y);
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define TO_GRID(x) ((x)/grid_step*grid_step)

const size_t DEFAULT_GRID = 8;
//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
	new EditorWidgets();
  current_widget_type = "";
	creating_status = 0;
	grid_step = DEFAULT_GRID;

	new MyGUI::Gui();
	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

  MyGUI::LayoutManager::getInstance().load("LayoutEditor.layout");

	ASSIGN_FUNCTION("buttonLoad", &EditorState::notifyLoad);
	ASSIGN_FUNCTION("buttonSave", &EditorState::notifySave);
	ASSIGN_FUNCTION("buttonSaveAs", &EditorState::notifySaveAs);
	ASSIGN_FUNCTION("buttonQuit", &EditorState::notifyQuit);

	ASSIGN_FUNCTION("widgetButton", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetEdit", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetList", &EditorState::notifyWidgetSelect);

	MyGUI::EditPtr gridEdit= MyGUI::Gui::getInstance().findWidget<MyGUI::Edit>("gridEdit");
	gridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyNewGridStepAccept);
	gridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);
}
//===================================================================================
bool EditorState::mouseMoved( const OIS::MouseEvent &arg )
{
	x2 = TO_GRID(arg.state.X.abs);
	y2 = TO_GRID(arg.state.Y.abs);

	if ((creating_status == 1) && ((x1-x2)*(y1-y2) != 0))
	{
		int x,y,w,h;
		x = min(x1, x2); y = min(y1, y2);
		w = abs(x1 - x2); h = abs(y1 - y2);
		
		creating_status = 2;
		std::string name = MyGUI::utility::toString(current_widget_type, counter, "_rectangle");
		current_widget = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>(current_widget_type, x, y, w, h, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Back", name);
		current_widget_rectangle = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("StretchRectangle", x, y, w, h, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped", name + "_rectangle");
	}
	else if (creating_status == 2)
	{
		int x,y,w,h;
		x = min(x1, x2); y = min(y1, y2);
		w = abs(x1 - x2); h = abs(y1 - y2);
		current_widget->setPosition(x, y, w, h);
		current_widget_rectangle->setPosition(x, y, w, h);
	}

	MyGUI::LayerItemInfoPtr rootItem = null;
	MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));

	if (null != item && item->getUserString("isInterface") == "")
	{
		if(item->getUserString("isMarker") != "") item = item->getParent();
		MyGUI::Gui::getInstance().findWidget<MyGUI::Edit>("propertyPositionEdit")->setCaption(item->getCoord().print());
	}

	MyGUI::InputManager::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool EditorState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	x1 = TO_GRID(arg.state.X.abs);
	y1 = TO_GRID(arg.state.Y.abs);

	if (id == OIS::MB_Left && !creating_status && current_widget_type != "") creating_status = 1;

	MyGUI::InputManager::getInstance().injectMousePress(arg, id);
	return true;
}
//===================================================================================
bool EditorState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	x2 = TO_GRID(arg.state.X.abs);
	y2 = TO_GRID(arg.state.Y.abs);

	if (creating_status > 0)
	{
		if ((x1-x2)*(y1-y2) != 0)
		{
			// создали виджет, все счастливы
			int x,y,w,h;
			x = min(x1, x2); y = min(y1, y2);
			w = abs(x1 - x2); h = abs(y1 - y2);
			creating_status = 0;
			current_widget_type = "";
			counter++;
			EditorWidgets::getInstance().add(current_widget->getName(), current_widget, current_widget_rectangle);
			current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
		}
		else
		{
			// не удалось создать, т.к. размер нулевой
			creating_status = 0;
			current_widget_type = "";
			if (current_widget) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
			if (current_widget_rectangle) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget_rectangle);
		}
	}

	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool EditorState::keyPressed( const OIS::KeyEvent &arg )
{
  if ( arg.key == OIS::KC_ESCAPE ) notifyQuit();

	MyGUI::InputManager::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool EditorState::keyReleased( const OIS::KeyEvent &arg )
{
	MyGUI::InputManager::getInstance().injectKeyRelease(arg);
	return true;
}
//===================================================================================
void EditorState::exit()
{
	MyGUI::Gui::getInstance().shutdown();
	delete MyGUI::Gui::getInstancePtr();
}
//===================================================================================
bool EditorState::frameStarted(const Ogre::FrameEvent& evt)
{
	MyGUI::Gui::getInstance().injectFrameEntered(evt.timeSinceLastFrame);
	return true;
}
//===================================================================================
void EditorState::notifyLoad(MyGUI::WidgetPtr _sender, bool _double)
{
	EditorWidgets::getInstance().load();
}

void EditorState::notifySave(MyGUI::WidgetPtr _sender, bool _double)
{
	EditorWidgets::getInstance().save();
}

void EditorState::notifySaveAs(MyGUI::WidgetPtr _sender, bool _double)
{
  
}

void EditorState::notifyQuit(MyGUI::WidgetPtr _sender, bool _double)
{
  BasisManager::getInstance().m_exit = true;
}

void EditorState::notifyWidgetSelect(MyGUI::WidgetPtr _sender, bool _double)
{
  current_widget_type = _sender->getCaption();
}

void EditorState::notifyNewGridStepAccept(MyGUI::WidgetPtr _sender)
{
	grid_step = Ogre::StringConverter::parseInt(_sender->getCaption());
	if (grid_step <= 0) grid_step = DEFAULT_GRID;
	_sender->setCaption(Ogre::StringConverter::toString(grid_step));
}

void EditorState::notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
{
	grid_step = Ogre::StringConverter::parseInt(_sender->getCaption());
	if (grid_step <= 0) grid_step = DEFAULT_GRID;
	_sender->setCaption(Ogre::StringConverter::toString(grid_step));
}

void EditorState::notifyRectangleResize(MyGUI::WidgetPtr _sender)
{
	// найдем соответствующий виджет и переместим/раст€нем
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_sender, 1);
	// подгон€ем под сетку
	MyGUI::IntCoord coord = _sender->getCoord();
	coord.left = TO_GRID(coord.left + grid_step-1);
	coord.top = TO_GRID(coord.top + grid_step-1);
	coord.width = TO_GRID(coord.width);
	coord.height = TO_GRID(coord.height);
	widgetContainer->widget->setPosition(coord);
	widgetContainer->widget_rectangle->setPosition(coord);
}