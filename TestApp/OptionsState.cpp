
#include "BasisManager.h"
#include "OptionsState.h"

#include "MyGUI_Gui.h"
#include "MyGUI_Window.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_List.h"
#include "MyGUI_ComboBox.h"
//#include "utility.h"

const size_t MAX_CREATE_WINDOW = 20;

using namespace MyGUI;

void OptionsState::enter(bool bIsChangeState)
{
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("MyGUI_main");
	material->load();

	mGUI = new MyGUI::Gui();
	mGUI->initialise(BasisManager::getInstance().mWindow);

	int height = (int)BasisManager::getInstance().mHeight;
	mFpsInfo = mGUI->createWidget<StaticText>("StaticText", 20, height - 80, 120, 70, ALIGN_LEFT | ALIGN_BOTTOM, "Main");
	mFpsInfo->setTextAlign(ALIGN_LEFT | ALIGN_TOP);

	mCountWindow = 0;

	ButtonPtr button = mGUI->createWidget<Button>("Button", IntCoord(10, 10, 200, 26), ALIGN_RIGHT | ALIGN_TOP, "Main");
	button->setCaption(L"demo list + combo");
	button->eventMouseButtonClick = newDelegate(this, &OptionsState::notifyWindowButton1);

	button = mGUI->createWidget<Button>("Button", 10, 46, 200, 26, ALIGN_RIGHT | ALIGN_TOP, "Main");
	button->setCaption(L"Edit demo + auto alpha");
	button->eventMouseButtonClick = newDelegate(this, &OptionsState::notifyWindowButton2);

	castWidget<Window>(button);
}

void OptionsState::exit()
{
	mGUI->shutdown();
	delete mGUI;
	mGUI = null;
}

bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	mGUI->injectMouseMove(arg);
	return true;
}

bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUI->injectMousePress(arg, id);
	return true;
}

bool OptionsState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUI->injectMouseRelease(arg, id);
	return true;
}

bool OptionsState::keyPressed( const OIS::KeyEvent &arg )
{
	mGUI->injectKeyPress(arg);
	return true;
}

bool OptionsState::keyReleased( const OIS::KeyEvent &arg )
{
	mGUI->injectKeyRelease(arg);
	return true;
}

bool OptionsState::frameStarted(const Ogre::FrameEvent& evt)
{
	static float time = 0;
	time += evt.timeSinceLastFrame;
	if (time > 1) {
		time -= 1;
		try {
			const Ogre::RenderTarget::FrameStats& stats = BasisManager::getInstance().mWindow->getStatistics();
			mFpsInfo->setCaption(util::toString("FPS : ", stats.lastFPS, "\ntriangle : ", stats.triangleCount, "\nbatch : ", stats.batchCount));
		} catch (...) { }
	}

	// добавляем время
	mGUI->injectFrameEntered(evt.timeSinceLastFrame);

	return true;
}

void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
}

void OptionsState::createWindowEdit()
{
	float x = (BasisManager::getInstance().mWidth - 300) * Ogre::Math::UnitRandom();
	float y = (BasisManager::getInstance().mHeight - 300) * Ogre::Math::UnitRandom();

	WindowPtr window = mGUI->createWidget<Window>("WindowCSX", x, y, 290, 300, ALIGN_LEFT | ALIGN_TOP, "Overlapped");
	window->setCaption("edit test");
	window->setAutoAlpha(true);
	window->showSmooth(true);
	window->setMinMax(MyGUI::IntRect(200, 110, 2000, 2000));
	window->eventWindowButtonPressed = newDelegate(this, &OptionsState::notifyWindowXPressed);

	EditPtr edit = window->createWidget<Edit>("EditStretch", 10, 44, 260, 175, ALIGN_STRETCH);
	edit->setEditMultiLine(true);

	ButtonPtr button = window->createWidget<Button>("ButtonSmall", 10, 10, 80, 24, ALIGN_LEFT | ALIGN_TOP);
	button->setCaption("multiline");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedMultiLine);
	button->setUserString("Edit", edit->getName());
	button->setColour(Ogre::ColourValue::White);

	MyGUI::ButtonPtr button2 = window->createWidget<MyGUI::Button>("ButtonSmall", 100, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button2->setCaption("read only");
	button2->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedReadOnly);
	button2->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", 190, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("password");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedPassword);
	button->setUserString("Edit", edit->getName());

	const MyGUI::IntCoord& coord = window->getClientRect();

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("green");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourGreen);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourGreen", button->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-180, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("red");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourRed);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourRed", button->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-270, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("blue");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourBlue);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourBlue", button->getName());
}

void OptionsState::notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	bool enable = true;
	float colour = 0.0f;
	if (edit->getEditReadOnly()) {
		edit->setEditReadOnly(false);
		_sender->setColour(Ogre::ColourValue::Black);

	}
	else {
		edit->setEditReadOnly(true);
		_sender->setColour(Ogre::ColourValue::White);
		enable = false;
		colour = 0.5f;
	}

	// кнопки управления цветом

	ButtonPtr button = mGUI->findWidget<Button>(_sender->getUserString("ColourBlue"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}
	button = mGUI->findWidget<Button>(_sender->getUserString("ColourGreen"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}
	button = mGUI->findWidget<Button>(_sender->getUserString("ColourRed"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}

}

void OptionsState::notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	if (edit->getEditPassword()) {
		edit->setEditPassword(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditPassword(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
}

void OptionsState::notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	if (edit->getEditMultiLine()) {
		edit->setEditMultiLine(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditMultiLine(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
}

void OptionsState::notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Green);
}

void OptionsState::notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Red);
}

void OptionsState::notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Blue);
}

//---------------------------------------------------------------------------------------//
// окно с листом
//---------------------------------------------------------------------------------------//
void OptionsState::createWindowList()
{
	float x = (BasisManager::getInstance().mWidth - 300) * Ogre::Math::UnitRandom();
	float y = (BasisManager::getInstance().mHeight - 300) * Ogre::Math::UnitRandom();

	VectorWidgetPtr roots = mGUI->loadLayout("ListDemo.layout");
	if (1 <= roots.size()) {
		WindowPtr window = castWidget<Window>(roots[0]);
		window->eventWindowButtonPressed = newDelegate(this, &OptionsState::notifyWindowXPressed);
		window->setPosition(IntPoint(x, y));
		window->showSmooth(true);

		VectorWidgetPtr childs = window->getChilds();
		if (4 <= childs.size()) {

			ListPtr list = castWidget<List>(childs[0]);
			list->eventListPressedDelete = newDelegate(this, &OptionsState::notifyListPressedDelete);

			ComboBoxPtr combo = castWidget<ComboBox>(childs[1]);
			combo->setUserString("List", list->getName());
			combo->eventComboAccept = newDelegate(this, &OptionsState::notifyEditAccept);

			ButtonPtr add = castWidget<Button>(childs[2]);
			add->setUserString("ComboBox", combo->getName());
			add->setUserString("List", list->getName());
			add->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedAdd);

			ButtonPtr del = castWidget<Button>(childs[3]);
			del->setUserString("List", list->getName());
			del->eventMouseButtonClick = newDelegate(this, &OptionsState::notifyPressedDelete);
		}
	}

/*	MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", x, y, 390, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped");
	window->setCaption("test list + combo");
	window->setMinMax(MyGUI::IntRect(200, 115, 2000, 2000));
	window->eventWindowButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyWindowXPressed);
	window->showSmooth(true);

	const MyGUI::IntCoord& coord = window->getClientRect();

	MyGUI::ListPtr list = window->createWidget<MyGUI::List>("List", 10, 46, coord.width-120, coord.height-56, MyGUI::ALIGN_STRETCH);
	list->eventListPressedDelete = MyGUI::newDelegate(this, &OptionsState::notifyListPressedDelete);

	MyGUI::ComboBoxPtr combo = window->createWidget<MyGUI::ComboBox>("ComboBox", 10, 10, coord.width-120, 26, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
	combo->setTextAlign(MyGUI::ALIGN_LEFT | MyGUI::ALIGN_VCENTER);
	combo->setUserString("List", list->getName());
	combo->eventComboAccept = MyGUI::newDelegate(this, &OptionsState::notifyEditAccept);
	combo->addItemString("#FF0000red");
	combo->addItemString("#00FF00green");
	combo->addItemString("#0000FFblue");
	combo->addItemString("#FFFF00yellow");
	combo->addItemString("#00FFFFcyan");
	combo->addItemString("#FF00FFmagenta");
	combo->addItemString("#FFFFFFwhite");
	combo->addItemString("#000000black");

	MyGUI::WidgetPtr separator = window->createWidget<MyGUI::Widget>("Separator3", coord.width-101, 10, 2, coord.height-20, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_VSTRETCH);

	MyGUI::ButtonPtr 	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, 10, 80, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_TOP);
	button->setCaption("add");
	button->setUserString("ComboBox", combo->getName());
	button->setUserString("List", list->getName());
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedAdd);

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, 46, 80, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_TOP);
	button->setCaption("delete");
	button->setUserString("List", list->getName());
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedDelete);
*/
}

void OptionsState::notifyPressedAdd(MyGUI::WidgetPtr _sender, bool _double)
{
	ComboBoxPtr combo = mGUI->findWidget<ComboBox>(_sender->getUserString("ComboBox"));
	if (combo == null) return;

	ListPtr list = mGUI->findWidget<List>(_sender->getUserString("List"));
	if (list == null) return;

	const Ogre::DisplayString& caption = combo->getCaption();
	if (false == caption.empty()) {
		list->addItemString(caption);
		combo->setCaption("");
	}
}

void OptionsState::notifyPressedDelete(MyGUI::WidgetPtr _sender, bool _double)
{
	ListPtr list = mGUI->findWidget<List>(_sender->getUserString("List"));
	if (list == null) return;

	size_t select = list->getItemSelect();
	if (select != ITEM_NONE) {
		list->deleteItemString(select);
	}
}

void OptionsState::notifyEditAccept(MyGUI::WidgetPtr _sender)
{
	ListPtr list = mGUI->findWidget<List>(_sender->getUserString("List"));
	if (list == null) return;

	const Ogre::DisplayString& caption = _sender->getCaption();
	if (false == caption.empty()) {
		list->addItemString(caption);
		_sender->setCaption("");
	}
}
	
void OptionsState::notifyListPressedDelete(MyGUI::WidgetPtr _sender)
{
	ListPtr list = castWidget<List>(_sender);

	size_t select = list->getItemSelect();
	if (select != ITEM_NONE) list->deleteItemString(select);
}

void OptionsState::notifyWindowXPressed(MyGUI::WidgetPtr _widget, const std::string& _name)
{
	WindowPtr window = castWidget<Window>(_widget);
	window->destroySmooth();
	mCountWindow --;
}

void OptionsState::notifyWindowButton1(MyGUI::WidgetPtr _sender, bool _double)
{
	if ((false == _double) && (mCountWindow < MAX_CREATE_WINDOW)) {
		createWindowList();
		mCountWindow ++;
	}
}

void OptionsState::notifyWindowButton2(MyGUI::WidgetPtr _sender, bool _double)
{
	if ((false == _double) && (mCountWindow < MAX_CREATE_WINDOW)) {
		createWindowEdit();
		mCountWindow ++;
	}
}

void OptionsState::comboAccept(MyGUI::WidgetPtr _sender)
{
	ComboBoxPtr combo = castWidget<ComboBox>(_sender);
	//MyGUI::MYGUI_OUT(combo->getCaption());
}

void OptionsState::updateWidgetPosition(Ogre::Entity * _entity, Ogre::Camera * _camera, MyGUI::WidgetPtr _widget)
{

	// get the projection of the object's AABB into screen space
	const Ogre::AxisAlignedBox& bbox = _entity->getWorldBoundingBox(true);
	Ogre::Matrix4 mat = _camera->getViewMatrix();

	const Ogre::Vector3* corners = bbox.getAllCorners();

	float min_x = 1.0f, max_x = 0.0f, min_y = 1.0f, max_y = 0.0f;

	// expand the screen-space bounding-box so that it completely encloses 
	// the object's AABB
	for (int i=0; i<8; i++) {
		Ogre::Vector3 corner = corners[i];

		// multiply the AABB corner vertex by the view matrix to 
		// get a camera-space vertex
		corner = mat * corner;

		// make 2D relative/normalized coords from the view-space vertex
		// by dividing out the Z (depth) factor -- this is an approximation
		float x = corner.x / corner.z + 0.5;
		float y = corner.y / corner.z + 0.5;

		if (x < min_x) min_x = x;
		if (x > max_x) max_x = x;
		if (y < min_y) min_y = y;
		if (y > max_y) max_y = y;
	}

	Ogre::Viewport * view = _camera->getViewport();

	// корректируем координаты для нормально восприятия
	// точка отсчета левый верхний угол
	float tmp = min_x;
	min_x = (1 - max_x);
	max_x = (1 - tmp);

	// середина ентити - половина длинны виджета
	int x = (int)((min_x + ((max_x - min_x) * 0.5)) * view->getActualWidth());
	x -= (_widget->getWidth() / 2);

	// верх энтити - высота виджета
	int y = (int)(min_y * view->getActualHeight());
	y -= _widget->getHeight();

	_widget->setPosition(x, y);

}
