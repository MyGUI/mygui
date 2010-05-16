/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/
#include "AwesomiumWidgetFactory.h"
#include "AwesomiumWidget.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_FactoryManager.h"

namespace Awesomium
{
	Awesomium::WebCore* AwesomiumWidgetFactory::msCore = 0;

	AwesomiumWidgetFactory::AwesomiumWidgetFactory() :
		mKeyboardHook(0)
	{
		msCore = new Awesomium::WebCore(Awesomium::LOG_VERBOSE);
		mKeyboardHook = new KeyboardHook(this);

		MyGUI::FactoryManager::getInstance().registerFactory<Awesomium::AwesomiumWidget>("Widget");

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &AwesomiumWidgetFactory::notifyFrameStart);
	}

	AwesomiumWidgetFactory::~AwesomiumWidgetFactory()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &AwesomiumWidgetFactory::notifyFrameStart);

		MyGUI::FactoryManager::getInstance().unregisterFactory<Awesomium::AwesomiumWidget>("Widget");

		delete mKeyboardHook;
		mKeyboardHook = 0;
		delete msCore;
		msCore = 0;
	}

	void AwesomiumWidgetFactory::notifyFrameStart(float _time)
	{
		if (msCore)
			msCore->update();
	}

	void AwesomiumWidgetFactory::handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		MyGUI::Widget* widget = MyGUI::InputManager::getInstance().getKeyFocusWidget();
		if (widget)
		{
			AwesomiumWidget* awidget = widget->castType<AwesomiumWidget>(false);
			if (awidget)
				awidget->getControl()->injectKeyboardEvent(hwnd, msg, wParam, lParam);
		}
	}

} // namespace Awesomium
