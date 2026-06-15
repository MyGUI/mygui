/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_FACTORY_MANAGER_H_
#define MYGUI_FACTORY_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_IObject.h"
#include "MyGUI_GenericFactory.h"

namespace MyGUI
{

	class MYGUI_EXPORT FactoryManager
	{
		MYGUI_SINGLETON_DECLARATION(FactoryManager);

	public:
		FactoryManager();

		void initialise();
		void shutdown();

		using Delegate = delegates::Delegate<IObject*&>;
		/** Register delegate function that creates object for specified _category and _type. */
		void registerFactory(std::string_view _category, std::string_view _type, Delegate::IDelegate* _delegate);
		/** Unregister delegate function that creates object for specified _category and _type. */
		void unregisterFactory(std::string_view _category, std::string_view _type);
		/** Unregister all delegate functions that creates object for specified _category. */
		void unregisterFactory(std::string_view _category);

		/** Is factory for specified _category and _type exist. */
		bool isFactoryExist(std::string_view _category, std::string_view _type);

		/** Register factory for specified _category. */
		template<typename Type>
		void registerFactory(std::string_view _category)
		{
			registerFactory(_category, Type::getClassTypeName(), GenericFactory<Type>::getFactory());
		}

		/** Register factory for specified _category and _type. */
		template<typename Type>
		void registerFactory(std::string_view _category, std::string_view _type)
		{
			registerFactory(_category, _type, GenericFactory<Type>::getFactory());
		}

		/** Unegister factory for specified _category. */
		template<typename Type>
		void unregisterFactory(std::string_view _category)
		{
			unregisterFactory(_category, Type::getClassTypeName());
		}

		/** Create object with given _category and _type. Return nullptr if there's no registered factory. */
		IObject* createObject(std::string_view _category, std::string_view _type);
		/** Create object with given _category and _type. Return nullptr if there's no registered factory. */
		template<typename Type>
		Type* createObject(std::string_view _category)
		{
			IObject* item = createObject(_category, Type::getClassTypeName());
			if (item != nullptr)
				return item->castType<Type>(false);
			return nullptr;
		}

		/** Destroy object */
		void destroyObject(IObject* _object);

	private:
		using MapFactoryItem = std::map<std::string, Delegate, std::less<>>;
		using MapRegisterFactoryItem = std::map<std::string, MapFactoryItem, std::less<>>;
		MapRegisterFactoryItem mRegisterFactoryItems;

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_FACTORY_MANAGER_H_
