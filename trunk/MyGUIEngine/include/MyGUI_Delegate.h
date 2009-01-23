/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_DELEGATE_H__
#define __MYGUI_DELEGATE_H__

#include <assert.h>
#include <typeinfo>
#include <list>

// source
// http://rsdn.ru/article/cpp/delegates.xml

// генерация делегатов для различного колличества параметров
namespace MyGUI
{

	namespace delegates
	{
		// базовый класс для тех классов, что хотят себя отвязывать от мульти делегатов
		class MYGUI_EXPORT IDelegateUnlink
		{
		public:
			IDelegateUnlink() { m_baseDelegateUnlink = this; }
			bool compare(IDelegateUnlink * _unlink) { return m_baseDelegateUnlink == _unlink->m_baseDelegateUnlink; }

		private:
			IDelegateUnlink * m_baseDelegateUnlink;
		};

		inline IDelegateUnlink * GetDelegateUnlink(void * _base) { return 0; }
		inline IDelegateUnlink * GetDelegateUnlink(IDelegateUnlink * _base) { return _base; }
	}

	// без параметров
	#define MYGUI_SUFFIX       0
	#define MYGUI_TEMPLATE
	#define MYGUI_TEMPLATE_PARAMS
	#define MYGUI_TEMPLATE_ARGS
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T>
	#define MYGUI_T_TEMPLATE_ARGS <T>
	#define MYGUI_PARAMS
	#define MYGUI_ARGS
	#define MYGUI_TYPENAME

	#include "MyGUI_DelegateImplement.h"

	// один параметр
	#define MYGUI_SUFFIX       1
	#define MYGUI_TEMPLATE	template
	#define MYGUI_TEMPLATE_PARAMS  <typename TP1>
	#define MYGUI_TEMPLATE_ARGS    <TP1>
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T, typename TP1>
	#define MYGUI_T_TEMPLATE_ARGS    <T, TP1>
	#define MYGUI_PARAMS       TP1 p1
	#define MYGUI_ARGS         p1
	#define MYGUI_TYPENAME     typename

	#include "MyGUI_DelegateImplement.h"

	// два параметра
	#define MYGUI_SUFFIX       2
	#define MYGUI_TEMPLATE	template
	#define MYGUI_TEMPLATE_PARAMS  <typename TP1, typename TP2>
	#define MYGUI_TEMPLATE_ARGS    <TP1, TP2>
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T, typename TP1, typename TP2>
	#define MYGUI_T_TEMPLATE_ARGS    <T, TP1, TP2>
	#define MYGUI_PARAMS       TP1 p1, TP2 p2
	#define MYGUI_ARGS         p1, p2
	#define MYGUI_TYPENAME     typename

	#include "MyGUI_DelegateImplement.h"

	// три параметра
	#define MYGUI_SUFFIX       3
	#define MYGUI_TEMPLATE	template
	#define MYGUI_TEMPLATE_PARAMS  <typename TP1, typename TP2, typename TP3>
	#define MYGUI_TEMPLATE_ARGS    <TP1, TP2, TP3>
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T, typename TP1, typename TP2, typename TP3>
	#define MYGUI_T_TEMPLATE_ARGS    <T, TP1, TP2, TP3>
	#define MYGUI_PARAMS       TP1 p1, TP2 p2, TP3 p3
	#define MYGUI_ARGS         p1, p2, p3
	#define MYGUI_TYPENAME     typename

	#include "MyGUI_DelegateImplement.h"

	// четыре параметра
	#define MYGUI_SUFFIX       4
	#define MYGUI_TEMPLATE	template
	#define MYGUI_TEMPLATE_PARAMS  <typename TP1, typename TP2, typename TP3, typename TP4>
	#define MYGUI_TEMPLATE_ARGS    <TP1, TP2, TP3, TP4>
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T, typename TP1, typename TP2, typename TP3, typename TP4>
	#define MYGUI_T_TEMPLATE_ARGS    <T, TP1, TP2, TP3, TP4>
	#define MYGUI_PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4
	#define MYGUI_ARGS         p1, p2, p3, p4
	#define MYGUI_TYPENAME     typename

	#include "MyGUI_DelegateImplement.h"

	// пять параметров
	#define MYGUI_SUFFIX       5
	#define MYGUI_TEMPLATE	template
	#define MYGUI_TEMPLATE_PARAMS  <typename TP1, typename TP2, typename TP3, typename TP4, typename TP5>
	#define MYGUI_TEMPLATE_ARGS    <TP1, TP2, TP3, TP4, TP5>
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T, typename TP1, typename TP2, typename TP3, typename TP4, typename TP5>
	#define MYGUI_T_TEMPLATE_ARGS    <T, TP1, TP2, TP3, TP4, TP5>
	#define MYGUI_PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5
	#define MYGUI_ARGS         p1, p2, p3, p4, p5
	#define MYGUI_TYPENAME     typename

	#include "MyGUI_DelegateImplement.h"


} // namespace MyGUI

#endif // __MYGUI_DELEGATE_H__
