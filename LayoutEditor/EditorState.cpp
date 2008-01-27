
#include "BasisManager.h"
#include "EditorState.h"
#include "WidgetContainer.h"

#include "MyGUI.h"

#define ASSIGN_FUNCTION(x,y) MyGUI::WidgetManager::getInstance().findWidgetT(x)->eventMouseButtonClick = MyGUI::newDelegate(this, y);
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define TO_GRID(x) ((x)/grid_step*grid_step)

const size_t DEFAULT_GRID = 8;

EditorWidgets * ew;
MyGUI::Gui * mGUI;
//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
	ew = new EditorWidgets();
  current_widget_type = "";
	creating_status = 0;
	grid_step = DEFAULT_GRID;
	fileName = "";

	mGUI = new MyGUI::Gui();
	mGUI->initialise(BasisManager::getInstance().mWindow);

  MyGUI::LayoutManager::getInstance().load("LayoutEditor.layout");

	ASSIGN_FUNCTION("buttonLoad", &EditorState::notifyLoadSaveAs);
	ASSIGN_FUNCTION("buttonSave", &EditorState::notifySave);
	ASSIGN_FUNCTION("buttonSaveAs", &EditorState::notifyLoadSaveAs);
	ASSIGN_FUNCTION("buttonQuit", &EditorState::notifyQuit);

	ASSIGN_FUNCTION("widgetButton", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetComboBox", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetEdit", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetHScroll", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetList", &EditorState::notifyWidgetSelect);
	ASSIGN_FUNCTION("widgetWindow", &EditorState::notifyWidgetSelect);

	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("gridEdit");
	gridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyNewGridStepAccept);
	gridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);
}
//===================================================================================
void EditorState::exit()
{
	mGUI->shutdown();
	delete mGUI;
	ew->shutdown();
	delete ew;
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
		current_widget = mGUI->createWidget<MyGUI::Window>(current_widget_type, x, y, w, h, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Back", name);
		current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", x, y, w, h, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped", name + "_rectangle");
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
		mGUI->findWidget<MyGUI::Edit>("propertyPositionEdit")->setCaption(item->getCoord().print());
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
			// ������� ������, ��� ���������
			int x,y,w,h;
			x = min(x1, x2); y = min(y1, y2);
			w = abs(x1 - x2); h = abs(y1 - y2);
			creating_status = 0;
			current_widget_type = "";
			counter++;
			ew->add(current_widget->getName(), current_widget, current_widget_rectangle);
			current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
		}
		else
		{
			// �� ������� �������, �.�. ������ �������
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
bool EditorState::frameStarted(const Ogre::FrameEvent& evt)
{
	mGUI->injectFrameEntered(evt.timeSinceLastFrame);
	return true;
}
//===================================================================================
void EditorState::notifySave(MyGUI::WidgetPtr _sender, bool _double)
{
	if (fileName != "") ew->save(fileName);
	else notifyLoadSaveAs(_sender, _double);
}

void EditorState::notifyLoadSaveAs(MyGUI::WidgetPtr _sender, bool _double)
{
	// create message box with file name and two buttons
  MyGUI::WidgetPtr messageWindow = MyGUI::LayoutManager::getInstance().load("LayoutEditorSaveLoad.layout")[0];
	MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
	MyGUI::IntSize size(messageWindow->getSize());
	messageWindow->setPosition((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);
	MyGUI::InputManager::getInstance().addWidgetModal(messageWindow);
	
	MyGUI::VectorWidgetPtr childs = messageWindow->getChilds();
	// set fileName in edit

	if (fileName != "") childs[0]->setCaption(fileName);
	if (_sender->getCaption() == "SaveAs...") childs[1]->setCaption("Save");
	else childs[1]->setCaption(_sender->getCaption());
	childs[1]->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveAccept);
	childs[2]->eventMouseButtonClick = newDelegate(this, &EditorState::notifyLoadSaveCancel);
}

void EditorState::notifyQuit(MyGUI::WidgetPtr _sender, bool _double)
{
  BasisManager::getInstance().m_exit = true;
}

void EditorState::notifyLoadSaveAccept(MyGUI::WidgetPtr _sender, bool _double)
{
  if (_sender->getCaption() == "Load") ew->load(mGUI->findWidget<MyGUI::Edit>("editFileName")->getCaption());
  else/*(_sender->getCaption() == "Save")*/ ew->save(mGUI->findWidget<MyGUI::Edit>("editFileName")->getCaption());
}

void EditorState::notifyLoadSaveCancel(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::InputManager::getInstance().removeWidgetModal(_sender->getParent()->getParent());
	MyGUI::WidgetManager::getInstance().destroyWidget(_sender->getParent()->getParent());
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
	// ������ ��������������� ������ � ����������/��������
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_sender, 1);
	// ��������� ��� �����
	MyGUI::IntCoord coord = _sender->getCoord();
	coord.left = TO_GRID(coord.left + grid_step-1);
	coord.top = TO_GRID(coord.top + grid_step-1);
	coord.width = TO_GRID(coord.width);
	coord.height = TO_GRID(coord.height);
	widgetContainer->widget->setPosition(coord);
	widgetContainer->widget_rectangle->setPosition(coord);
}