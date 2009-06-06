/*!
    @file
    @author     Albert Semenov
    @date       06/2009
    @module
*/
#ifndef __MYGUI_GENERIC_FACTORY_H__
#define __MYGUI_GENERIC_FACTORY_H__

#include <MyGUI.h>
#include "MyGUI_Types.h"

namespace MyGUI
{

	template <typename BaseType>
	class GenericFactory
	{
	public:
		typedef delegates::CDelegate3<BaseType*&, xml::ElementPtr, Version> DelegateFactory;

		void registerFactoryItem(const std::string& _name, typename DelegateFactory::IDelegate* _delegate)
		{
			mFactoryItems[_name] = _delegate;
		}

		BaseType* createFromFactory(const std::string& _type, xml::ElementPtr _node, Version _version)
		{
			MapDelegate::iterator item = mFactoryItems.find(_type);
			if (item == mFactoryItems.end()) return nullptr;
			BaseType* result = nullptr;
			item->second(result, _node, _version);
			return result;
		}

	private:
		typedef std::map<std::string, DelegateFactory> MapDelegate;
		MapDelegate mFactoryItems;
	};

}

#endif // __MYGUI_GENERIC_FACTORY_H__
