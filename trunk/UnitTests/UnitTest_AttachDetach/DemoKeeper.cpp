/*!
	@file
	@author		Albert Semenov
	@date		12/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	MyGUI::VectorWidgetPtr all_widgets;

	void test_widgets()
	{
	}

	int random(int _max)
	{
		int result = rand() % _max;
		return result < 0 ? 0 : result;
	}

	MyGUI::WidgetStyle get_type()
	{
		const int SIZE = 3;
		static MyGUI::WidgetStyle types[SIZE] = { MyGUI::WidgetStyle::Child,  MyGUI::WidgetStyle::Popup,  MyGUI::WidgetStyle::Overlapped };
		return types[random(SIZE)];
	}

	void erase_widget(MyGUI::VectorWidgetPtr& _mass, MyGUI::Widget* _widget)
	{
		for (MyGUI::VectorWidgetPtr::iterator iter = _mass.begin(); iter != _mass.end(); ++iter)
		{
			if (*iter == _widget)
			{
				*iter = _mass.back();
				_mass.pop_back();
				return;
			}
		}
	}

	MyGUI::Widget* get_random(MyGUI::VectorWidgetPtr& _mass)
	{
		if (_mass.empty()) return 0;
		return _mass.at(random((int)_mass.size()));
	}

	const char* get_skin()
	{
		const int SIZE = 8;
		static const char* names[SIZE] = { "WindowCSX", "ScrollView", "ButtonX", "ButtonV", "Button", "EditStretch", "RadioBox", "CheckBox" };
		return names[random(SIZE)];
	}

	const char* get_layer()
	{
		const int SIZE = 4;
		static const char* names[SIZE] = { "", "Main", "Overlapped", "Popup" };
		return names[random(SIZE)];
	}

	MyGUI::IntCoord get_coord()
	{
		return MyGUI::IntCoord(random(500), random(500), random(500), random(500));
	}

	void step_detach_layer()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (!widget) return;
		MyGUI::LayerManager::getInstance().detachFromLayer(widget);
		test_widgets();
	}

	void step_detach_layer(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_detach_layer();
			--count;
		};
	}

	void step_attach_layer()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (!widget) return;
		if (widget->isRootWidget())
		{
			std::string layername = get_layer();
			if (!layername.empty())
				MyGUI::LayerManager::getInstance().attachToLayerNode(layername, widget);
		}
		test_widgets();
	}

	void step_attach_layer(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_attach_layer();
			--count;
		};
	}

	void step_detach_widget()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (!widget) return;
		widget->detachFromWidget(get_layer());
		test_widgets();
	}

	void step_detach_widget(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_detach_widget();
			--count;
		};
	}

	void step_attach_widget()
	{
		MyGUI::Widget* widget1 = get_random(all_widgets);
		MyGUI::Widget* widget2 = get_random(all_widgets);
		if (!widget1 || !widget2) return;

		MyGUI::Widget* test = widget1;
		do
		{
			if (test == widget2) return;
			test = test->getParent();
		}
		while (test);

		widget2->attachToWidget(widget1, get_type(), get_layer());
		test_widgets();
	}

	void step_attach_widget(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_attach_widget();
			--count;
		};
	}

	void step_destroy_widget()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (!widget) return;
		/*if (!widget->isRootWidget()) */
		MyGUI::WidgetManager::getInstance().destroyWidget(widget);
		test_widgets();
	}

	void step_destroy_widget(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_destroy_widget();
			--count;
		};
	}

	void step_create_widget()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (widget)
		{
			int select = random(3);
			if (select == 0)
			{
				MyGUI::Widget* child = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Child, get_skin(), get_coord(), MyGUI::Align::Default);
				MYGUI_ASSERT(child, "child nullptr");
				all_widgets.push_back(child);
			}
			else if (select == 1)
			{
				MyGUI::Widget* child = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Popup, get_skin(), get_coord(), MyGUI::Align::Default, get_layer());
				MYGUI_ASSERT(child, "child nullptr");
				all_widgets.push_back(child);
			}
			else if (select == 2)
			{
				MyGUI::Widget* child = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Overlapped, get_skin(), get_coord(), MyGUI::Align::Default);
				MYGUI_ASSERT(child, "child nullptr");
				all_widgets.push_back(child);
			}
		}
		else
		{
			MyGUI::Widget* child = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(get_skin(), get_coord(), MyGUI::Align::Default, get_layer());
			MYGUI_ASSERT(child, "child nullptr");
			all_widgets.push_back(child);
		}
		test_widgets();
	}

	void step_create_widget(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_create_widget();
			--count;
		};
	}

	void step_change_skin()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (!widget) return;
		widget->changeWidgetSkin(get_skin());
		test_widgets();
	}

	void step_change_skin(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_change_skin();
			--count;
		};
	}

	void step_change_type()
	{
		MyGUI::Widget* widget = get_random(all_widgets);
		if (!widget) return;
		widget->setWidgetStyle(get_type());
		test_widgets();
	}

	void step_change_type(int _count)
	{
		int count = random(_count);
		while (count > 0)
		{
			step_change_type();
			--count;
		};
	}

	class Unlink :
		public MyGUI::IUnlinkWidget
	{
	public:
		void _unlinkWidget(MyGUI::Widget* _widget)
		{
			erase_widget(all_widgets, _widget);
		}
	};

	Unlink unlink_holder;

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::WidgetManager::getInstance().registerUnlinker(&unlink_holder);
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);
		MyGUI::WidgetManager::getInstance().unregisterUnlinker(&unlink_holder);
	}

	void DemoKeeper::notifyFrameStarted(float _time)
	{
		if (all_widgets.size() > 500)
		{
			step_destroy_widget(200);
		}
		else
		{
			int step = random(8);
			if (step == 0)
			{
				step_detach_layer(10);
			}
			else if (step == 1)
			{
				step_attach_layer(30);
			}
			else if (step == 2)
			{
				step_attach_widget(10);
			}
			else if (step == 3)
			{
				step_detach_widget(10);
			}
			else if (step == 4)
			{
				step_destroy_widget(2);
			}
			else if (step == 5)
			{
				step_create_widget(30);
			}
			else if (step == 6)
			{
				step_change_skin(30);
			}
			else if (step == 7)
			{
				step_change_type(30);
			}
		}

		getStatisticInfo()->change("COUNT", all_widgets.size());

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(10);
#endif

	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
