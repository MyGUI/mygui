/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

// дополнительная инициализация
#define MMYGUI_INITIALISE

// использовать некоторые внешние типы
//#define MMYGUI_USING_EXTERNAL_TYPE

#ifdef MMYGUI_USING_EXTERNAL_TYPE
// неймспейс внешних типов
#    define MMYGUI_EXTERNAL_NAMESPACE
#else
// неймспейс внешних типов
#    define MMYGUI_EXTERNAL_NAMESPACE
#endif

//------------------------------------------------------------------------------//
#define MMYGUI_CHECK_NATIVE(ptr) \
	if (ptr == nullptr) throw gcnew System::NullReferenceException();
