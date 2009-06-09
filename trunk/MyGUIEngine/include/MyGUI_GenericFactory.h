/*!
    @file
    @author     Albert Semenov
    @date       06/2009
    @module
*/
#ifndef __MYGUI_GENERIC_FACTORY_H__
#define __MYGUI_GENERIC_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_FactoryManager.h"

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
		static void _createFromFactory(IObject*& _instance)
		{
			_instance = new Type();
		}
	};

}

#endif // __MYGUI_GENERIC_FACTORY_H__
