/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_ProgressFactory.h"
#include "MyGUI_Progress.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ProgressFactory::ProgressFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("Progress_Range") = newDelegate(this, &ProgressFactory::Progress_Range);
			manager.registerDelegate("Progress_Position") = newDelegate(this, &ProgressFactory::Progress_Position);
			manager.registerDelegate("Progress_AutoTrack") = newDelegate(this, &ProgressFactory::Progress_AutoTrack);
			manager.registerDelegate("Progress_StartPoint") = newDelegate(this, &ProgressFactory::Progress_StartPoint);
		}

		ProgressFactory::~ProgressFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Progress_Range");
			manager.unregisterDelegate("Progress_Position");
			manager.unregisterDelegate("Progress_AutoTrack");
			manager.unregisterDelegate("Progress_StartPoint");
		}

		const std::string& ProgressFactory::getType()
		{
			return Progress::_getType();
		}

		WidgetPtr ProgressFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name)
		{
			return new Progress(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		// методы для парсинга
		void ProgressFactory::Progress_Range(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ProgressPtr, _widget, _key);
			static_cast<ProgressPtr>(_widget)->setProgressRange(utility::parseSizeT(_value));
		}

		void ProgressFactory::Progress_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ProgressPtr, _widget, _key);
			static_cast<ProgressPtr>(_widget)->setProgressPosition(utility::parseSizeT(_value));
		}

		void ProgressFactory::Progress_AutoTrack(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ProgressPtr, _widget, _key);
			static_cast<ProgressPtr>(_widget)->setProgressAutoTrack(utility::parseBool(_value));
		}

		void ProgressFactory::Progress_StartPoint(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ProgressPtr, _widget, _key);
			static_cast<ProgressPtr>(_widget)->setProgressStartPoint(Align::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
