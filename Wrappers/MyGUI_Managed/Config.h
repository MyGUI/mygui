/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

// additional initialization
#define MMYGUI_INITIALISE

// use some external types
//#define MMYGUI_USING_EXTERNAL_TYPE

#ifdef MMYGUI_USING_EXTERNAL_TYPE
// namespace of external types
#    define MMYGUI_EXTERNAL_NAMESPACE
#else
// namespace of external types
#    define MMYGUI_EXTERNAL_NAMESPACE
#endif

//------------------------------------------------------------------------------//
#define MMYGUI_CHECK_NATIVE(ptr) \
	if (ptr == nullptr) throw gcnew System::NullReferenceException();
