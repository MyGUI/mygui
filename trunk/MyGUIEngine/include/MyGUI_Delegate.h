/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_DELEGATE_H__
#define __MYGUI_DELEGATE_H__

#include <assert.h>

// source
// http://rsdn.ru/article/cpp/delegates.xml

// генерация делегатов для различного колличества параметров
namespace MyGUI
{

	// базовый класс для тех классов, что хотят себя отвязывать от мульти делегатов
	class _MyGUIExport IDelegateUnlink
	{
	public:
		IDelegateUnlink() { m_baseDelegateUnlink = this; }
		bool compare(IDelegateUnlink * _unlink) { return m_baseDelegateUnlink == _unlink->m_baseDelegateUnlink; }

	private:
		IDelegateUnlink * m_baseDelegateUnlink;
	};

	inline IDelegateUnlink * GetDelegateUnlink(void * _base) { return 0; }
	inline IDelegateUnlink * GetDelegateUnlink(IDelegateUnlink * _base) { return _base; }

	// один параметр
	#define SUFFIX       1
	#define TEMPLATE_PARAMS  typename TP1
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
	#define TEMPLATE_PARAMS  typename TP1, typename TP2
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
	#define TEMPLATE_PARAMS  typename TP1, typename TP2, typename TP3
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
	#define TEMPLATE_PARAMS  typename TP1, typename TP2, typename TP3, typename TP4
	#define TEMPLATE_ARGS    TP1, TP2, TP3, TP4
	#define PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4
	#define ARGS         p1, p2, p3, p4

	#include "MyGUI_DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

	// пять параметров
	#define SUFFIX       5
	#define TEMPLATE_PARAMS  typename TP1, typename TP2, typename TP3, typename TP4, typename TP5
	#define TEMPLATE_ARGS    TP1, TP2, TP3, TP4, TP5
	#define PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5
	#define ARGS         p1, p2, p3, p4, p5

	#include "MyGUI_DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

} // namespace MyGUI

#endif // __MYGUI_DELEGATE_H__
