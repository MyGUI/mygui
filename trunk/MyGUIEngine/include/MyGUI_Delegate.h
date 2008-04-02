/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_DELEGATE_H__
#define __MYGUI_DELEGATE_H__

// source
// http://rsdn.ru/article/cpp/delegates.xml

// генерация делегатов для различного колличества параметров

namespace MyGUI
{

	// один параметр
	#define SUFFIX       1
	#define TEMPLATE_PARAMS  class TP1
	#define TEMPLATE_ARGS    TP1
	#define PARAMS       TP1 p1
	#define ARGS         p1

	#include "MyGUI_DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

	// два параметра
	#define SUFFIX       2
	#define TEMPLATE_PARAMS  class TP1, class TP2
	#define TEMPLATE_ARGS    TP1, TP2
	#define PARAMS       TP1 p1, TP2 p2
	#define ARGS         p1, p2

	#include "MyGUI_DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

	// три параметра
	#define SUFFIX       3
	#define TEMPLATE_PARAMS  class TP1, class TP2, class TP3
	#define TEMPLATE_ARGS    TP1, TP2, TP3
	#define PARAMS       TP1 p1, TP2 p2, TP3 p3
	#define ARGS         p1, p2, p3

	#include "MyGUI_DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

	// четыре параметра
	#define SUFFIX       4
	#define TEMPLATE_PARAMS  class TP1, class TP2, class TP3, class TP4
	#define TEMPLATE_ARGS    TP1, TP2, TP3, TP4
	#define PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4
	#define ARGS         p1, p2, p3, p4

	#include "MyGUI_DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

} // namespace MyGUI

#endif // __MYGUI_DELEGATE_H__
