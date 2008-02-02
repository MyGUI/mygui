
#include "BasisManager.h"
#include "EditorState.h"
#include "WidgetContainer.h"
#include "Types.h"

#include "MyGUI.h"

#define ASSIGN_FUNCTION(x,y) MyGUI::WidgetManager::getInstance().findWidgetT(x)->eventMouseButtonClick = MyGUI::newDelegate(this, y);
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define TO_GRID(x) ((x)/grid_step*grid_step)

const size_t DEFAULT_GRID = 8;

EditorWidgets * ew;
MyGUI::Gui * mGUI;

MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget)
{
	MyGUI::WidgetPtr parent = widget->getParent();
	if (null != parent){
		if (parent->getWidgetType() == "Widget")// если наш папа - Window
		{
			parent = parent->getParent();
			coord = coord - parent->getPosition() - MyGUI::IntPoint(parent->getClientRect().left,parent->getClientRect().top);
		}else coord = coord - parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		coord = convertCoordToParentCoord(coord, parent);
	}
	return coord;
}

MyGUI::IntCoord convertParentCoordToCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget)
{
	MyGUI::WidgetPtr parent = widget->getParent();
	if (null != parent){
		if (parent->getWidgetType() == "Widget")// если наш папа - Window
		{
			parent = parent->getParent();
			coord = coord + parent->getPosition() + MyGUI::IntPoint(parent->getClientRect().left,parent->getClientRect().top);
		}else coord = coord + parent->getPosition();
		// а может у нас и дедушка есть? а может и прадед...
		coord = convertParentCoordToCoord(coord, parent);
	}
	return coord;
}

//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
	ew = new EditorWidgets();
	ew->initialise();
	current_widget_type = "";
	current_widget_skin = "";
	creating_status = 0;
	grid_step = DEFAULT_GRID;
	fileName = "";

	mGUI = new MyGUI::Gui();
	mGUI->initialise(BasisManager::getInstance().mWindow);

	MyGUI::LayoutManager::getInstance().load("interface.layout");

	// menu panel (should be dropdown menu)
	ASSIGN_FUNCTION("buttonLoad", &EditorState::notifyLoadSaveAs);
	ASSIGN_FUNCTION("buttonSave", &EditorState::notifySave);
	ASSIGN_FUNCTION("buttonSaveAs", &EditorState::notifyLoadSaveAs);
	ASSIGN_FUNCTION("buttonQuit", &EditorState::notifyQuit);

	// widgets panel
	MyGUI::WindowPtr windowWidgets = mGUI->findWidget<MyGUI::Window>("windowWidgets");
	for (int i = 0; i<NUM_WIDGETS; i++)
	{
		MyGUI::ButtonPtr button = windowWidgets->createWidgetReal<MyGUI::Button>("Button", 0. + i%2*0.5, 0. + i/2*0.1, 0.5, 0.1, MyGUI::ALIGN_DEFAULT);
		button->setCaption(widget_types[i].widget + " " + widget_types[i].skin);
		button->setUserString("widget", widget_types[i].widget);
		button->setUserString("skin", widget_types[i].skin);
		button->eventMouseButtonClick = MyGUI::newDelegate(this, &EditorState::notifySelectWidgetType);
	}

	// editor settings panel
	MyGUI::EditPtr gridEdit= mGUI->findWidget<MyGUI::Edit>("gridEdit");
	gridEdit->eventEditSelectAccept = MyGUI::newDelegate(this, &EditorState::notifyNewGridStepAccept);
	gridEdit->eventKeyLostFocus = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);

	// rectangle for widgets moving/resizing
	//current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", IntCoord(), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Popup");
	//current_widget_rectangle->hide();
	//current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
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
		MyGUI::IntCoord coord(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));

		creating_status = 2;
		std::string name = MyGUI::utility::toString(current_widget_skin, counter);

		// внимание current_widget родитель и потом сразу же сын
		if (current_widget)
		{
			coord = coord - current_widget->getPosition();
			current_widget = current_widget->createWidgetT(current_widget_type, current_widget_skin, coord, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, name);
		}
		else current_widget = mGUI->createWidgetT(current_widget_type, current_widget_skin, coord, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Back", name);

		current_widget->setCaption(name);
		// сделаю, если все виджеты будут реагировать на такое событие
		//current_widget->eventMouseButtonClick = newDelegate(this, &EditorState::notifySelectWidget);
	}
	else if (creating_status == 2)
	{
		MyGUI::IntCoord coord(min(x1, x2), min(y1, y2), abs(x1 - x2), abs(y1 - y2));
		coord = convertCoordToParentCoord(coord, current_widget);
		current_widget->setPosition(coord);
	}

	MyGUI::LayerItemInfoPtr rootItem = null;
	MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));

	if (null != ew->find(item))
	{
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
	//else
	{
		// select current winget and create StretchRectangle for it

		MyGUI::LayerItemInfoPtr rootItem = null;
		MyGUI::WidgetPtr item = static_cast<MyGUI::WidgetPtr>(MyGUI::LayerManager::getInstance().findWidgetItem(arg.state.X.abs, arg.state.Y.abs, rootItem));

		if (null != ew->find(item))
		{
			notifySelectWidget(item);
			if (creating_status != 1) MyGUI::InputManager::getInstance().injectMouseMove(arg); // это чтобы сразу можно было тащить
		}
		else if ((item != current_widget_rectangle) &&
			(!item || (item && !item->getParent() || (item->getParent() && item->getParent() != current_widget_rectangle))))
		{
			if (current_widget_rectangle)
			{
				MyGUI::WidgetManager::getInstance().destroyWidget(current_widget_rectangle);
				current_widget_rectangle = null;
			}
			current_widget = null;
		}
	}

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
			ew->add(new WidgetContainer(current_widget->getName(), current_widget_type, current_widget_skin, current_widget));
			notifySelectWidget(current_widget);
			creating_status = 0;
			current_widget_type = "";
			current_widget_skin = "";
			counter++;
		}
		else
		{
			// не удалось создать, т.к. размер нулевой
			creating_status = 0;
			current_widget_type = "";
			if ((creating_status > 1) && current_widget) MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
		}
	}

	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool EditorState::keyPressed( const OIS::KeyEvent &arg )
{
	//if ( arg.key == OIS::KC_ESCAPE ) notifyQuit();
	if ( arg.key == OIS::KC_SYSRQ ) {BasisManager::getInstance().mWindow->writeContentsToFile("screenshot.png");}

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
	MyGUI::WidgetPtr messageWindow = MyGUI::LayoutManager::getInstance().load("SaveLoadMessage.layout")[0];
	MyGUI::IntSize view((int)mGUI->getViewWidth(), (int)mGUI->getViewHeight());
	MyGUI::IntSize size(messageWindow->getSize());
	messageWindow->setPosition((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);
	MyGUI::InputManager::getInstance().addWidgetModal(messageWindow);
	
	MyGUI::VectorWidgetPtr childs = messageWindow->getChilds();

	// set fileName in edit
	if (fileName != "") childs[0]->setCaption(fileName);
	MyGUI::castWidget<MyGUI::Edit>(childs[0])->eventEditSelectAccept = newDelegate(this, &EditorState::notifyLoadSaveEditAccept);
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
	bool success;
	if (_sender->getCaption() == "Load") success = ew->load(mGUI->findWidget<MyGUI::Edit>("editFileName")->getCaption());
	else/*(_sender->getCaption() == "Save")*/ success = ew->save(mGUI->findWidget<MyGUI::Edit>("editFileName")->getCaption());

	if (false == success) 
	{
		MyGUI::Message::createMessage("Warning", "Failed to " + _sender->getCaption() + " file", 1, MyGUI::Message::IconWarning | MyGUI::Message::Ok);
	}
	else
	{
		// запоминает последнее удачное имя файла
		fileName = mGUI->findWidget<MyGUI::Edit>("editFileName")->getCaption();
		notifyLoadSaveCancel(_sender);
	}
}

void EditorState::notifyLoadSaveEditAccept(MyGUI::WidgetPtr _widget)
{
	notifyLoadSaveAccept(mGUI->findWidgetT("buttonSaveLoad"));
}

void EditorState::notifyLoadSaveCancel(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::InputManager::getInstance().removeWidgetModal(mGUI->findWidgetT("windowSaveLoad"));
	MyGUI::WidgetManager::getInstance().destroyWidget(mGUI->findWidgetT("windowSaveLoad"));
}

void EditorState::notifySelectWidgetType(MyGUI::WidgetPtr _sender, bool _double)
{
	current_widget_type = _sender->getUserString("widget");
	current_widget_skin = _sender->getUserString("skin");
}

void EditorState::notifyNewGridStep(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
{
	grid_step = Ogre::StringConverter::parseInt(_sender->getCaption());
	if (grid_step <= 0) grid_step = DEFAULT_GRID;
	_sender->setCaption(Ogre::StringConverter::toString(grid_step));
}

void EditorState::notifyNewGridStepAccept(MyGUI::WidgetPtr _sender)
{
	notifyNewGridStep();
}

void EditorState::notifySelectWidget(MyGUI::WidgetPtr _sender, bool _double)
{
	current_widget = _sender;
	if (current_widget_rectangle)
	{
		MyGUI::WidgetManager::getInstance().destroyWidget(current_widget_rectangle);
		current_widget_rectangle = null;
	}

	MyGUI::IntCoord coord = _sender->getCoord();

	MyGUI::WidgetPtr parent = _sender->getParent();
	if (null == parent) current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", coord, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Popup");
	else{
		coord = convertParentCoordToCoord(coord, _sender);
		current_widget_rectangle = mGUI->createWidget<MyGUI::Window>("StretchRectangle", coord, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Popup");
	}
	current_widget_rectangle->eventWindowChangeCoord = newDelegate(this, &EditorState::notifyRectangleResize);
}

void EditorState::notifyRectangleResize(MyGUI::WidgetPtr _sender)
{
	// найдем соответствующий виджет и переместим/растянем
	WidgetContainer * widgetContainer = ew->find(current_widget);
	// align to grid
	MyGUI::IntCoord coord = _sender->getCoord();
	coord.left = TO_GRID(coord.left + grid_step-1);
	coord.top = TO_GRID(coord.top + grid_step-1);
	coord.width = TO_GRID(coord.width);
	coord.height = TO_GRID(coord.height);
	current_widget_rectangle->setPosition(coord);

	coord = convertCoordToParentCoord(coord, current_widget);
	current_widget->setPosition(coord);
}