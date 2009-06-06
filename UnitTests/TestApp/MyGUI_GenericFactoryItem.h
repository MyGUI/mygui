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

	template <typename BaseType, typename DerivedType>
	class GenericFactoryItem
	{
	public:
		typedef delegates::CDelegate3<BaseType*&, xml::ElementPtr, Version> DelegateFactory;

		static typename DelegateFactory::IDelegate* createFactory()
		{
			return newDelegate(createFromFactory);
		}

	private:
		static void createFromFactory(BaseType*& _instance, xml::ElementPtr _node, Version _version)
		{
			_instance = new DerivedType(_node, _version);
		}
	};

}

#endif // __MYGUI_GENERIC_FACTORY_ITEM_H__
