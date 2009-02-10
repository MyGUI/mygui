/*!
	@file
	@author		Albert Semenov
	@date
	@module
*/
#pragma once

#include "MMyGUI_Macros.h"
#include <vcclr.h>

MMYGUI_BEGIN_NAMESPACE

ref class MMYGUI_WIDGET_NAME;
class WidgetHolder
{
public:
	WidgetHolder() : object() { }
	WidgetHolder(MMYGUI_WIDGET_NAME ^ _obj) : object(_obj) { }
	~WidgetHolder() { }
	MMYGUI_WIDGET_NAME ^ toObject() { return object; }
private:
	gcroot<MMYGUI_WIDGET_NAME^> object;
};

MMYGUI_END_NAMESPACE
