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
		return;
		// ��� ������� ����, � �������� ���������� ���� ��������, ���� �� 0,
		// �� �� ����� � ��� �� �����
		MyGUI::LayerKeeper * layer = _widget->getLayerKeeper();

		// ��� ��� �����, �.�. ����� ��������, ���� ����� ���������������, �� ������� ���� ������,
		// ���� ���, �� ���� ������� �� ���� ��� � ���� ����
		MyGUI::LayerItemKeeper * layer_item = _widget->getLayerItemKeeper();

		// �� ���
		if (layer) {

			if (!_widget->isRootWidget()) {
				MYGUI_EXCEPT("layer != null && !isRootWidget()");
			}

			if (!layer_item) {
				MYGUI_EXCEPT("layer != null && layer_item == null");
			}

			// ��������� ������������ ������ � ��� ������
			bool exist = layer->existItem(layer_item);
			if (!exist) {
				MYGUI_EXCEPT("layer item is not exist");
			}

		}
		// �� �� ���
		else {
			if (layer_item) {
				// ���� �������� ����
				MyGUI::ICroppedRectangle * parent = _widget->getCroppedParent();
				if (!parent) {
					MYGUI_EXCEPT("cropped parent == null");
				}

				while (parent->getCroppedParent()) {

					// � �� �����, �� ������ ���� �����
					MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();
					if (layer3) {
						MYGUI_EXCEPT("layer != null");
					}

					parent = parent->getCroppedParent();
				};
				MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();

				// � ���� ������ ���� �����
				if (!layer3) {
					MYGUI_EXCEPT("layer == null");
				}

				// ��������� ������������ ������ � ��� ������
				bool exist = layer3->existItem(layer_item);
				if (!exist) {
					MYGUI_EXCEPT("layer item is not exist");
				}
				
			}
			// �� ����������
			else {
			}

			// ��������� ��� �� ������ ���� ����������
		}
	}

	void test_widgets()
	{
		Ogre::MemoryManager & manager = Ogre::MemoryManager::instance();
		for (MyGUI::VectorWidgetPtr::iterator iter = all_widgets.begin(); iter!=all_widgets.end(); ++iter) {
			// ��������� �� ������� �� ��� ������
			//MYGUI_CHECK_PTR(*iter);
			diagnosticRenderItem(*iter);
		}
	}

	int random(int _max)
	{
		int result = rand() % _max;
		return result < 0 ? 0 : result;
	}

	MyGUI::WidgetType get_type()
	{
		const int SIZE = 3;
		static MyGUI::WidgetType types[SIZE] = { MyGUI::WidgetType::Child,  MyGUI::WidgetType::Popup,  MyGUI::WidgetType::Overlapped };
		return types[random(SIZE)];
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
		MyGUI::LayerManager::getInstance().detachFromLayerKeeper(widget);
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
		if (widget->isRootWidget())
		{
			std::string layername = get_layer();
			if (!layername.empty())
				MyGUI::LayerManager::getInstance().attachToLayerKeeper(layername, widget);
		}
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
		//widget->detachFromWidget();
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

		//widget2->attachToWidget(widget1);
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
		/*if (!widget->isRootWidget()) */MyGUI::WidgetManager::getInstance().destroyWidget(widget);
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
			int select = random(3);
			if (select == 0) {
				MyGUI::WidgetPtr child = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetType::Child, get_skin(), get_coord(), MyGUI::Align::Default);
				MYGUI_ASSERT(child, "child null");
				all_widgets.push_back(child);
			}
			else if (select == 1) {
				MyGUI::WidgetPtr child = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetType::Popup, get_skin(), get_coord(), MyGUI::Align::Default, get_layer());
				MYGUI_ASSERT(child, "child null");
				all_widgets.push_back(child);
			}
			else if (select == 2) {
				//MyGUI::WidgetPtr child = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetType::Overlapped, get_skin(), get_coord(), MyGUI::Align::Default);
				//MYGUI_ASSERT(child, "child null");
				//all_widgets.push_back(child);
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
		if (!widget) return;
		widget->changeWidgetSkin(get_skin());
		test_widgets();
	}

	void step_change_skin(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_change_skin(); --count; };
	}

	void step_change_type()
	{
		MyGUI::WidgetPtr widget = get_random(all_widgets);
		if (!widget) return;
		widget->setWidgetType(get_type());
		test_widgets();
	}

	void step_change_type(int _count)
	{
		int count = random(_count);
		while (count > 0) { step_change_type(); --count; };
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
			int step = random(8);
			if (step == 0) {
				step_detach_layer(10);
			}
			else if (step == 1) {
				step_attach_layer(30);
			}
			else if (step == 2) {
				//step_attach_widget(10);
			}
			else if (step == 3) {
				//step_detach_widget(10);
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
			else if (step == 7) {
				//step_change_type(30);
			}
		}

		mInfo->change("COUNT", all_widgets.size());

		MyGUI::EnumeratorLayerKeeperPtr layer = MyGUI::LayerManager::getInstance().getEnumerator();
		while (layer.next()) {
			size_t count = layer->getItemCount();
			if (count > 0)
			{
				mInfo->change(layer->getName(), count);
			}
		}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(1);
#endif

		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo
