/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#ifndef __MYGUI_RENDER_BOX_FACTORY_H__
#define __MYGUI_RENDER_BOX_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_RenderBox.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT RenderBoxFactory : public BaseWidgetFactory<RenderBox>
		{
		public:
			RenderBoxFactory();
			~RenderBoxFactory();
		private:
			// методы для парсинга
			void RenderBox_Mesh(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_AutoRotation(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_AutoRotationSpeed(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_BackgroungColour(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_RotationAngle(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_MouseRotation(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_Animation(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void RenderBox_ViewScale(WidgetPtr _widget, const std::string &_key, const std::string &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_FACTORY_H__
