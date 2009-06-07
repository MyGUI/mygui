/*!
    @file
    @author     Albert Semenov
    @date       06/2009
    @module
*/
#ifndef __MYGUI_GENERIC_FACTORY_ITEM_H__
#define __MYGUI_GENERIC_FACTORY_ITEM_H__

#include <MyGUI.h>
#include "MyGUI_Types.h"

namespace MyGUI
{

	template <typename Type>
	class GenericFactory
	{	
	public:
		static typename FactoryManager::Delegate::IDelegate* getFactory()
		{
			return newDelegate(_createFromFactory);
		}

	private:
		static void _createFromFactory(Object*& _instance)
		{
			_instance = new Type();
		}
	};

}

#endif // __MYGUI_GENERIC_FACTORY_ITEM_H__
