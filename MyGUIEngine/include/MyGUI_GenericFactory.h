/*!
	@file
	@author     Albert Semenov
	@date       06/2009
*/
#ifndef __MYGUI_GENERIC_FACTORY_H__
#define __MYGUI_GENERIC_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_Delegate.h"

namespace MyGUI
{

	template <typename Type>
	class GenericFactory
	{
	public:
		typedef delegates::CDelegate1<IObject*&> Delegate;
		static typename Delegate::IDelegate* getFactory()
		{
			return newDelegate(createFromFactory);
		}

	private:
		static void createFromFactory(IObject*& _instance)
		{
			_instance = new Type();
		}
	};

	struct RegisterType
	{
		template <typename U, U> struct test_struct { };
		template <typename Type> static void CallStaticConstructor(test_struct<void(*)(), Type::staticConstructor>* a)
		{
			Type::staticConstructor();
		}
		template <typename Type> static void CallStaticConstructor(...)
		{
		}
		template <typename Type> static void CallStaticDestructor(test_struct<void(*)(), Type::staticDestructor>* a)
		{
			Type::staticDestructor();
		}
		template <typename Type> static void CallStaticDestructor(...)
		{
		}
	};

}

#endif // __MYGUI_GENERIC_FACTORY_H__
