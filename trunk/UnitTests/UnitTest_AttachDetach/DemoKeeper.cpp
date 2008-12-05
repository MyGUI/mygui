/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	MyGUI::VectorWidgetPtr all_widgets;

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
		const int SIZE = 2;
		static const char * names[SIZE] = {"ButtonX", "ButtonV"};
		return names[random(SIZE)];
	}

	const char * get_layer()
	{
		const int SIZE = 2;
		static const char * names[SIZE] = {"Main", "Overlapped"};
		return names[random(SIZE)];
	}

	MyGUI::IntCoord get_coord()
	{
		return MyGUI::IntCoord(random(500), random(500), random(500), random(500));
	}

	void step_detach()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		widget->detachWidget();
	}

	void step_detach(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_detach(); --count; };
	}

	void step_attach_layer()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		widget->attachWidget(get_layer());
	}

	void step_attach_layer(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_attach_layer(); --count; };
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

		widget2->attachWidget(widget1);
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
	}

	void step_create_widget(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_create_widget(); --count; };
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
			int step = random(5);
			if (step == 0) {
				step_detach(10);
			}
			else if (step == 1) {
				//step_attach_layer(10);
			}
			else if (step == 2) {
				//step_attach_widget(10);
			}
			else if (step == 3) {
				step_destroy_widget(2);
			}
			else if (step == 4) {
				step_create_widget(30);
			}
		}

		mInfo->change("COUNT", all_widgets.size());


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(1);
#endif

		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo
