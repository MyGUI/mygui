/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __DELEGATE_H__
#define __DELEGATE_H__

// source
// http://rsdn.ru/article/cpp/delegates.xml

// генерация делегатов для различного колличества параметров
namespace delegates
{

	// базовый класс для тех классов, что хотят себя отвязывать от мульти делегатов
	class DelegateUnlink;
	class DelegateUnlink
	{
	public:
		friend class DelegateUnlink;
	public:
		inline DelegateUnlink() { m_baseDelegateUnlink = this; }
		inline bool compare(DelegateUnlink * _unlink) { return m_baseDelegateUnlink == _unlink->m_baseDelegateUnlink; }

	private:
		DelegateUnlink * m_baseDelegateUnlink;
	};

	inline DelegateUnlink * GetDelegateUnlink(void * _base) { return 0; }
	inline DelegateUnlink * GetDelegateUnlink(DelegateUnlink * _base) { return _base; }

	// один параметр
	#define SUFFIX       1
	#define TEMPLATE_PARAMS  class TP1
	#define TEMPLATE_ARGS    TP1
	#define PARAMS       TP1 p1
	#define ARGS         p1

	#include "DelegateImplement.h"

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

	#include "DelegateImplement.h"

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

	#include "DelegateImplement.h"

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

	#include "DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

	// пять параметров
	#define SUFFIX       5
	#define TEMPLATE_PARAMS  class TP1, class TP2, class TP3, class TP4, class TP5
	#define TEMPLATE_ARGS    TP1, TP2, TP3, TP4, TP5
	#define PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5
	#define ARGS         p1, p2, p3, p4, p5

	#include "DelegateImplement.h"

	#undef SUFFIX
	#undef TEMPLATE_PARAMS
	#undef TEMPLATE_ARGS
	#undef PARAMS
	#undef ARGS

} // namespace delegates

#endif // __DELEGATE_H__
