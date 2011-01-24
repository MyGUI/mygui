/*!
	@file
	@author		George Evmenov
	@date		01/2011
*/
#ifndef __FUNCTOR_DELEGATE_H__
#define __FUNCTOR_DELEGATE_H__

#include "MyGUI_Diagnostic.h"
#include <typeinfo>
#include <list>

namespace MyGUI
{

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

	#include "FunctorDelegateImplementation.h"

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

	#include "FunctorDelegateImplementation.h"

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

	#include "FunctorDelegateImplementation.h"

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

	#include "FunctorDelegateImplementation.h"

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

	#include "FunctorDelegateImplementation.h"

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

	#include "FunctorDelegateImplementation.h"

	// шесть параметров
	#define MYGUI_SUFFIX       6
	#define MYGUI_TEMPLATE	template
	#define MYGUI_TEMPLATE_PARAMS  <typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6>
	#define MYGUI_TEMPLATE_ARGS    <TP1, TP2, TP3, TP4, TP5, TP6>
	#define MYGUI_T_TEMPLATE_PARAMS  <typename T, typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6>
	#define MYGUI_T_TEMPLATE_ARGS    <T, TP1, TP2, TP3, TP4, TP5, TP6>
	#define MYGUI_PARAMS       TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6
	#define MYGUI_ARGS         p1, p2, p3, p4, p5, p6
	#define MYGUI_TYPENAME     typename

	#include "FunctorDelegateImplementation.h"


} // namespace MyGUI

#endif // __MYGUI_DELEGATE_H__
