/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
#include "HikariWidget.h"
#include <objbase.h>

namespace Hikari
{
	namespace factory
	{

		HMODULE HikariWidgetFactory::msFlashLib = 0;

		HikariWidgetFactory::HikariWidgetFactory()
		{
			CoInitialize(0);
			msFlashLib = LoadLibraryA("Flash.ocx");
			MyGUI::WidgetManager::getInstance().registerFactory(this);

			MyGUI::WidgetManager& manager = MyGUI::WidgetManager::getInstance();
			manager.registerDelegate("Hikari_Source") = newDelegate(this, &HikariWidgetFactory::Hikari_Source);
			manager.registerDelegate("Hikari_Transparent") = newDelegate(this, &HikariWidgetFactory::Hikari_Transparent);
		}

		HikariWidgetFactory::~HikariWidgetFactory()
		{
			MyGUI::WidgetManager& manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterDelegate("Hikari_Transparent");
			manager.unregisterDelegate("Hikari_Source");

			MyGUI::WidgetManager::getInstance().unregisterFactory(this);
			if (msFlashLib)
			{
				FreeLibrary(msFlashLib);
				msFlashLib = 0;
			}
			CoUninitialize();
		}

		const std::string& HikariWidgetFactory::getTypeName()
		{
			return HikariWidget::getClassTypeName();
		}

		MyGUI::WidgetPtr HikariWidgetFactory::createWidget(MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle* _croppedParent, MyGUI::IWidgetCreator* _creator, const std::string& _name)
		{
			return new HikariWidget(_style, _coord, _align, MyGUI::SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
		}

		bool HikariWidgetFactory::isFalseType(MyGUI::WidgetPtr _ptr, const std::string &_key)
		{
			if (!_ptr->isType<HikariWidget>())
			{
				MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
				return true;
			}
			return false;
		}

		void HikariWidgetFactory::Hikari_Source(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<HikariWidget*>(_widget)->load(_value);
		}

		void HikariWidgetFactory::Hikari_Transparent(MyGUI::WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<HikariWidget*>(_widget)->setTransparent(MyGUI::utility::parseBool(_value));
		}

	} // namespace factory

} // namespace Hikari
