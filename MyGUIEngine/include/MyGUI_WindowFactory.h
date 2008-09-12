/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WINDOW_FACTORY_H__
#define __MYGUI_WINDOW_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport WindowFactory : public WidgetFactoryInterface
		{
		public:
			WindowFactory();
			~WindowFactory();

			const std::string& getType();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name);

			void Window_AutoAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Window_Snap(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Window_MinMax(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Window_MinSize(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Window_MaxSize(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_WINDOW_FACTORY_H__
