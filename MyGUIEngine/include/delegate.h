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

// ��������� ��������� ��� ���������� ����������� ����������

// ���� ��������
#define SUFFIX       1
#define TEMPLATE_PARAMS  class TP1
#define TEMPLATE_ARGS    TP1
#define PARAMS       TP1 p1
#define ARGS         p1

#include "delegate_implement.h"

#undef SUFFIX
#undef TEMPLATE_PARAMS
#undef TEMPLATE_ARGS
#undef PARAMS
#undef ARGS

// ��� ���������
#define SUFFIX       2
#define TEMPLATE_PARAMS  class TP1, class TP2
#define TEMPLATE_ARGS    TP1, TP2
#define PARAMS       TP1 p1, TP2 p2
#define ARGS         p1, p2

#include "delegate_implement.h"

#undef SUFFIX
#undef TEMPLATE_PARAMS
#undef TEMPLATE_ARGS
#undef PARAMS
#undef ARGS

// ��� ���������
#define SUFFIX       3
#define TEMPLATE_PARAMS  class TP1, class TP2, class TP3
#define TEMPLATE_ARGS    TP1, TP2, TP3
#define PARAMS       TP1 p1, TP2 p2, TP3 p3
#define ARGS         p1, p2, p3

#include "delegate_implement.h"

#undef SUFFIX
#undef TEMPLATE_PARAMS
#undef TEMPLATE_ARGS
#undef PARAMS
#undef ARGS

#endif
