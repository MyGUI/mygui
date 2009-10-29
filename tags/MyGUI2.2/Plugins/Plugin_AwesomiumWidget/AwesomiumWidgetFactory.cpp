/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
#include "AwesomiumWidgetFactory.h"
#include "AwesomiumWidget.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_InputManager.h"

namespace Awesomium
{
	namespace factory
	{

		Awesomium::WebCore* AwesomiumWidgetFactory::msCore = 0;

		AwesomiumWidgetFactory::AwesomiumWidgetFactory() :
			mKeyboardHook(0)
		{
			MyGUI::WidgetManager::getInstance().registerFactory(this);

			msCore = new Awesomium::WebCore(Awesomium::LOG_VERBOSE);
			mKeyboardHook = new KeyboardHook(this);

			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &AwesomiumWidgetFactory::notifyFrameStart);

			MyGUI::WidgetManager& manager = MyGUI::WidgetManager::getInstance();
			manager.registerDelegate("Awesomium_SourceURL") = newDelegate(this, &AwesomiumWidgetFactory::Awesomium_SourceURL);
			manager.registerDelegate("Awesomium_Transparent") = newDelegate(this, &AwesomiumWidgetFactory::Awesomium_Transparent);
		}

		AwesomiumWidgetFactory::~AwesomiumWidgetFactory()
		{
			MyGUI::WidgetManager& manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterDelegate("Awesomium_Transparent");
			manager.unregisterDelegate("Awesomium_SourceURL");

			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &AwesomiumWidgetFactory::notifyFrameStart);

			delete mKeyboardHook;
			mKeyboardHook = 0;
			delete msCore;
			msCore = 0;

			MyGUI::WidgetManager::getInstance().unregisterFactory(this);
		}

		const std::string& AwesomiumWidgetFactory::getTypeName()
		{
			return AwesomiumWidget::getClassTypeName();
		}

		MyGUI::WidgetPtr AwesomiumWidgetFactory::createWidget(MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle* _croppedParent, MyGUI::IWidgetCreator* _creator, const std::string& _name)
		{
			return new AwesomiumWidget(_style, _coord, _align, MyGUI::SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
		}

		bool AwesomiumWidgetFactory::isFalseType(MyGUI::WidgetPtr _ptr, const std::string &_key)
		{
			if (!_ptr->isType<AwesomiumWidget>())
			{
				MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
				return true;
			}
			return false;
		}

		void AwesomiumWidgetFactory::Awesomium_SourceURL(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<AwesomiumWidget*>(_widget)->loadURL(_value);
		}

		void AwesomiumWidgetFactory::Awesomium_Transparent(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<AwesomiumWidget*>(_widget)->setTransparent(MyGUI::utility::parseBool(_value));
		}

		void AwesomiumWidgetFactory::notifyFrameStart(float _time)
		{
			if (msCore)
				msCore->update();
		}

		void AwesomiumWidgetFactory::handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			MyGUI::WidgetPtr widget = MyGUI::InputManager::getInstance().getKeyFocusWidget();
			if (widget)
			{
				AwesomiumWidget* awidget = widget->castType<AwesomiumWidget>(false);
				if (awidget)
					awidget->getControl()->injectKeyboardEvent(hwnd, msg, wParam, lParam);
			}
		}

	} // namespace factory

} // namespace Awesomium
