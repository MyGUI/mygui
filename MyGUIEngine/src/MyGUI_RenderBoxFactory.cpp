/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_RenderBoxFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		RenderBoxFactory::RenderBoxFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("RenderBox_Mesh") = newDelegate(this, &RenderBoxFactory::RenderBox_Mesh);
			manager.registerDelegate("RenderBox_AutoRotation") = newDelegate(this, &RenderBoxFactory::RenderBox_AutoRotation);
			manager.registerDelegate("RenderBox_AutoRotationSpeed") = newDelegate(this, &RenderBoxFactory::RenderBox_AutoRotationSpeed);
			manager.registerDelegate("RenderBox_BackgroungColour") = newDelegate(this, &RenderBoxFactory::RenderBox_BackgroungColour);
			manager.registerDelegate("RenderBox_RotationAngle") = newDelegate(this, &RenderBoxFactory::RenderBox_RotationAngle);
			manager.registerDelegate("RenderBox_MouseRotation") = newDelegate(this, &RenderBoxFactory::RenderBox_MouseRotation);
			manager.registerDelegate("RenderBox_Animation") = newDelegate(this, &RenderBoxFactory::RenderBox_Animation);
			manager.registerDelegate("RenderBox_ViewScale") = newDelegate(this, &RenderBoxFactory::RenderBox_ViewScale);
		}

		RenderBoxFactory::~RenderBoxFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("RenderBox_Mesh");
			manager.unregisterDelegate("RenderBox_AutoRotation");
			manager.unregisterDelegate("RenderBox_AutoRotationSpeed");
			manager.unregisterDelegate("RenderBox_BackgroungColour");
			manager.unregisterDelegate("RenderBox_RotationAngle");
			manager.unregisterDelegate("RenderBox_MouseRotation");
			manager.unregisterDelegate("RenderBox_Animation");
			manager.unregisterDelegate("RenderBox_ViewScale");
		}

		void RenderBoxFactory::RenderBox_Mesh(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->injectObject(_value);
		}

		void RenderBoxFactory::RenderBox_AutoRotation(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setAutoRotation(utility::parseBool(_value));
		}

		void RenderBoxFactory::RenderBox_AutoRotationSpeed(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setAutoRotationSpeed(utility::parseInt(_value));
		}

		void RenderBoxFactory::RenderBox_BackgroungColour(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setBackgroungColour(Colour::parse(_value).toColourValue());
		}

		void RenderBoxFactory::RenderBox_RotationAngle(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setRotationAngle(Ogre::Degree(utility::parseInt(_value)));
		}

		void RenderBoxFactory::RenderBox_MouseRotation(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setMouseRotation(utility::parseBool(_value));
		}

		void RenderBoxFactory::RenderBox_Animation(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setAnimation(_value);
		}

		void RenderBoxFactory::RenderBox_ViewScale(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<RenderBoxPtr>(_widget)->setViewScale(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
