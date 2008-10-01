/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "MyGUI_ScrollViewFactory.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ScrollViewFactory::ScrollViewFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("ScrollView_VScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_VScroll);
			manager.registerDelegate("ScrollView_HScroll") = newDelegate(this, &ScrollViewFactory::ScrollView_HScroll);
			manager.registerDelegate("ScrollView_CanvasAlign") = newDelegate(this, &ScrollViewFactory::ScrollView_CanvasAlign);
			manager.registerDelegate("ScrollView_CanvasSize") = newDelegate(this, &ScrollViewFactory::ScrollView_CanvasSize);
		}

		ScrollViewFactory::~ScrollViewFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("ScrollView_VScroll");
			manager.unregisterDelegate("ScrollView_HScroll");
			manager.unregisterDelegate("ScrollView_CanvasAlign");
			manager.unregisterDelegate("ScrollView_CanvasSize");
		}

		const std::string & ScrollViewFactory::getTypeName()
		{
			return ScrollView::getClassTypeName();
		}

		WidgetPtr ScrollViewFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new ScrollView(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void ScrollViewFactory::ScrollView_VScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ScrollView, _widget, _key);
			static_cast<ScrollViewPtr>(_widget)->showVScroll(utility::parseBool(_value));
		}

		void ScrollViewFactory::ScrollView_HScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ScrollView, _widget, _key);
			static_cast<ScrollViewPtr>(_widget)->showHScroll(utility::parseBool(_value));
		}

		void ScrollViewFactory::ScrollView_CanvasAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ScrollView, _widget, _key);
			static_cast<ScrollViewPtr>(_widget)->setCanvasAlign(Align::parse(_value));
		}

		void ScrollViewFactory::ScrollView_CanvasSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ScrollView, _widget, _key);
			static_cast<ScrollViewPtr>(_widget)->setCanvasSize(IntSize::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
