#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class WidgetFactory
	{
	
	public:
		virtual const String & getType() = 0;
		virtual MyGUI::Window * createWidget(const String & _skin, uint16 _x, uint16 _y, uint16 _cx, uint16 _cy, MyGUI::Window * _parent, uint16 _aligin, uint16 _overlay) = 0; 
	};


	#define REGISTRY_WIDGET_FACTORY(_type,_class) \
		class FACTORY_##_type : public WidgetFactory \
		{ \
		public : \
			FACTORY_##_type() \
			{ \
				GUI::getSingleton()->addWidgetFactory(this); \
			} \
			const String & getType() \
			{ \
				return _type; \
			}; \
			MyGUI::Window * createWidget(const String & _skin, uint16 _x, uint16 _y, uint16 _cx, uint16 _cy, MyGUI::Window * _parent, uint16 _aligin, uint16 _overlay) \
			{ \
				return _class::createWindow(_x, _y, _cx, _cy, _parent, _aligin, _overlay, _skin); \
			} \
		}; \
		FACTORY_##_type FACTORY_INSTANCE_##_type;

} // namespace MyGUI {