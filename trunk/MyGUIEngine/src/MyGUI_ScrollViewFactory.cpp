/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "MyGUI_ScrollViewFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ScrollViewFactory::ScrollViewFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("ScrollView_VScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_VScroll);
			manager.registerDelegate("ScrollView_HScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_HScroll);
			manager.registerDelegate("ScrollView_CanvasAlign") = newDelegate(this, &ScrollViewFactory::ScrollView_CanvasAlign);
			manager.registerDelegate("ScrollView_CanvasSize") = newDelegate(this, &ScrollViewFactory::ScrollView_CanvasSize);
		}

		ScrollViewFactory::~ScrollViewFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("ScrollView_VScroll");
			manager.unregisterDelegate("ScrollView_HScroll");
			manager.unregisterDelegate("ScrollView_CanvasAlign");
			manager.unregisterDelegate("ScrollView_CanvasSize");
		}

		void ScrollViewFactory::ScrollView_VScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->showVScroll(utility::parseBool(_value));
		}

		void ScrollViewFactory::ScrollView_HScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->showHScroll(utility::parseBool(_value));
		}

		void ScrollViewFactory::ScrollView_CanvasAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->setCanvasAlign(Align::parse(_value));
		}

		void ScrollViewFactory::ScrollView_CanvasSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ScrollViewPtr>(_widget)->setCanvasSize(IntSize::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
