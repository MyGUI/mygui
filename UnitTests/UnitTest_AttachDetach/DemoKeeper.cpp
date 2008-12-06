/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#include "DemoKeeper.h"

#include "OgreMemoryManager.h"
#include "MyGUI_LayerKeeper.h"

namespace demo
{

	MyGUI::VectorWidgetPtr all_widgets;

	void diagnosticRenderItem(MyGUI::WidgetPtr _widget)
	{
		// это главный леер, к которому приатачена наша иерархия, если он 0,
		// ты мы висим и нас не видно
		MyGUI::LayerKeeper * layer = _widget->getLayerKeeper();

		// это наш айтем, т.е. некоя обертака, если кипер перекрывающийся, то обертка наша личная,
		// если нет, то одна обертка на всех кто в этом слое
		MyGUI::LayerItemKeeper * layer_item = _widget->getLayerItemKeeper();

		// мы рут
		if (layer) {

			if (!_widget->isRootWidget()) {
				MYGUI_EXCEPT("layer != null && !isRootWidget()");
			}

			if (!layer_item) {
				MYGUI_EXCEPT("layer != null && layer_item == null");
			}

			// проверяем соответствие кипера и его айтема
			bool exist = layer->existItem(layer_item);
			if (!exist) {
				MYGUI_EXCEPT("layer item is not exist");
			}

		}
		// мы не рут
		else {
			if (layer_item) {
				// ищем корневой леер
				MyGUI::ICroppedRectangle * parent = _widget->getCroppedParent();
				if (!parent) {
					MYGUI_EXCEPT("cropped parent == null");
				}

				while (parent->getCroppedParent()) {

					// у не рутов, не должен быть кипер
					MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();
					if (layer3) {
						MYGUI_EXCEPT("layer != null");
					}

					parent = parent->getCroppedParent();
				};
				MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();

				// у рута должен быть кипер
				if (!layer3) {
					MYGUI_EXCEPT("layer == null");
				}

				// проверяем соответствие кипера и его айтема
				bool exist = layer3->existItem(layer_item);
				if (!exist) {
					MYGUI_EXCEPT("layer item is not exist");
				}
				
			}
			// мы отдетачены
			else {
			}

			// проверяем все ли рендер дети отцепленны
		}
	}

	void test_widgets()
	{
		Ogre::MemoryManager & manager = Ogre::MemoryManager::instance();
		for (MyGUI::VectorWidgetPtr::iterator iter = all_widgets.begin(); iter!=all_widgets.end(); ++iter) {
			// проверяем не удалили ли уже виджет
			MYGUI_ASSERT(manager.validateAddr(*iter), "pointer is dead");
			diagnosticRenderItem(*iter);
		}
	}

	int random(int _max)
	{
		int result = rand() % _max;
		return result < 0 ? 0 : result;
	}

	void erase_widget(MyGUI::VectorWidgetPtr & _mass, MyGUI::WidgetPtr _widget)
	{
		for (MyGUI::VectorWidgetPtr::iterator iter = _mass.begin(); iter!=_mass.end(); ++iter) {
			if (*iter == _widget) {
				*iter = _mass.back();
				_mass.pop_back();
				return;
			}
		}
		//MYGUI_EXCEPT(MyGUI::utility::toString("pointer ", _widget, " not found");
	}

	MyGUI::WidgetPtr get_random(MyGUI::VectorWidgetPtr & _mass)
	{
		if (_mass.empty()) return 0;
		return _mass.at(random((int)_mass.size()));
	}

	const char * get_skin()
	{
		const int SIZE = 8;
		static const char * names[SIZE] = { "WindowCSX", "ScrollView", "ButtonX", "ButtonV" , "Button", "EditStretch", "RadioBox", "CheckBox" };
		return names[random(SIZE)];
	}

	const char * get_layer()
	{
		const int SIZE = 4;
		static const char * names[SIZE] = { "", "Main", "Overlapped", "Back" };
		return names[random(SIZE)];
	}

	MyGUI::IntCoord get_coord()
	{
		return MyGUI::IntCoord(random(500), random(500), random(500), random(500));
	}

	void step_detach_layer()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		widget->detachFromLayer();
		test_widgets();
	}

	void step_detach_layer(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_detach_layer(); --count; };
	}

	void step_attach_layer()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		widget->attachToLayer(get_layer());
		test_widgets();
	}

	void step_attach_layer(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_attach_layer(); --count; };
	}

	void step_detach_widget()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		widget->detachFromWidget();
		test_widgets();
	}

	void step_detach_widget(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_detach_widget(); --count; };
	}

	void step_attach_widget()
	{
		MyGUI::WidgetPtr widget1 = get_random(all_widgets);
		MyGUI::WidgetPtr widget2 = get_random(all_widgets);
		if (!widget1 || !widget2) return;

		MyGUI::WidgetPtr test = widget1;
		do {
			if (test == widget2) return;
			test = test->getParent();
		} while (test);

		widget2->attachToWidget(widget1);
		test_widgets();
	}

	void step_attach_widget(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_attach_widget(); --count; };
	}

	void step_destroy_widget()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		MyGUI::WidgetManager::getInstance().destroyWidget(widget);
		test_widgets();
	}

	void step_destroy_widget(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_destroy_widget(); --count; };
	}

	void step_create_widget()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (widget) {
			if (random(2)) {
				MyGUI::WidgetPtr child = widget->createWidget<MyGUI::Widget>(get_skin(), get_coord(), MyGUI::Align::Default);
				MYGUI_ASSERT(child, "child null");
				all_widgets.push_back(child);
			}
			else {
				MyGUI::WidgetPtr child = widget->createWidgetRoot<MyGUI::Widget>(get_skin(), get_coord(), MyGUI::Align::Default, get_layer());
				MYGUI_ASSERT(child, "child null");
				all_widgets.push_back(child);
			}
		}
		else {
			MyGUI::WidgetPtr child = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(get_skin(), get_coord(), MyGUI::Align::Default, get_layer());
			MYGUI_ASSERT(child, "child null");
			all_widgets.push_back(child);
		}
		test_widgets();
	}

	void step_create_widget(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_create_widget(); --count; };
	}

	void step_change_skin()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (widget) widget->changeWidgetSkin(get_skin());
		test_widgets();
	}

	void step_change_skin(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_change_skin(); --count; };
	}

	class Unlink : public MyGUI::IUnlinkWidget
	{
	public:
		void _unlinkWidget(MyGUI::WidgetPtr _widget)
		{
			erase_widget(all_widgets, _widget);
		}
	};

	Unlink unlink_holder;

	void DemoKeeper::createScene()
	{
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(100, 100);

		MyGUI::WidgetManager::getInstance().registerUnlinker(&unlink_holder);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::WidgetManager::getInstance().unregisterUnlinker(&unlink_holder);
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{

		if (all_widgets.size() > 500) {
			step_destroy_widget(100);
		}
		else {
			int step = random(7);
			if (step == 0) {
				step_detach_layer(30);
			}
			else if (step == 1) {
				step_attach_layer(10);
			}
			else if (step == 2) {
				step_attach_widget(10);
			}
			else if (step == 3) {
				step_detach_widget(10);
			}
			else if (step == 4) {
				step_destroy_widget(2);
			}
			else if (step == 5) {
				step_create_widget(30);
			}
			else if (step == 6) {
				step_change_skin(30);
			}
		}

		mInfo->change("COUNT", all_widgets.size());


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(1);
#endif

		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo
