/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_PROGRESS_FACTORY_H__
#define __MYGUI_PROGRESS_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_Progress.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT ProgressFactory : public BaseWidgetFactory<Progress>
		{
		public:
			ProgressFactory();
			~ProgressFactory();
		private:
			// методы для парсинга
			void Progress_Range(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Progress_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Progress_AutoTrack(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Progress_StartPoint(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_PROGRESS_FACTORY_H__
