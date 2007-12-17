
#include "BasisManager.h"
#include "EditorState.h"

#include "MyGUI_Window.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_List.h"
#include "MyGUI_StretchRectangle.h"

#define ASSIGN_FUNCTION(x,y) MyGUI::WidgetManager::getInstance().findWidgetT(x)->eventMouseButtonClick = MyGUI::newDelegate(this, y);
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define TO_GRID(x) (x/grid_step*grid_step)

const size_t DEFAULT_GRID = 8;
//===================================================================================
void EditorState::enter(bool bIsChangeState)
{
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

	MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Edit>("gridEdit")->eventEditTextChange = MyGUI::newDelegate(this, &EditorState::notifyNewGridStep);
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
		current_widget = MyGUI::Gui::getInstance().createWidget<MyGUI::StretchRectangle>(current_widget_type, x, y, w, h, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main");
	}
	else if (creating_status == 2)
	{
		int x,y,w,h;
		x = min(x1, x2); y = min(y1, y2);
		w = abs(x1 - x2); h = abs(y1 - y2);
    current_widget->setPosition(x, y, w, h);
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
			int x,y,w,h;
			x = min(x1, x2); y = min(y1, y2);
			w = abs(x1 - x2); h = abs(y1 - y2);
			MyGUI::Gui::getInstance().createWidgetT(current_widget_type, current_widget_type, x, y, w, h, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main", "");
			creating_status = 0;
			current_widget_type = "";
			MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
		}
		else
		{
			creating_status = 0;
			current_widget_type = "";
			MyGUI::WidgetManager::getInstance().destroyWidget(current_widget);
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
	return true;
}
//===================================================================================
void EditorState::notifyLoad(MyGUI::WidgetPtr _sender, bool _double)
{

}

void EditorState::notifySave(MyGUI::WidgetPtr _sender, bool _double)
{

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

void EditorState::notifyNewGridStep(MyGUI::WidgetPtr _sender)
{
	grid_step = Ogre::StringConverter::parseInt(_sender->getCaption());
	if (grid_step <= 0)
	{
		grid_step = DEFAULT_GRID;
		_sender->setCaption(Ogre::StringConverter::toString(DEFAULT_GRID));
	}
}