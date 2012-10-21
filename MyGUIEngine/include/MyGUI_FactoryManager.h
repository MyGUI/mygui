/*!
	@file
	@author		Albert Semenov
	@date		06/2009
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_FACTORY_MANAGER_H__
#define __MYGUI_FACTORY_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_IObject.h"
#include "MyGUI_GenericFactory.h"

namespace MyGUI
{

	class MYGUI_EXPORT FactoryManager :
		public Singleton<FactoryManager>
	{
	public:
		FactoryManager();

		void initialise();
		void shutdown();

		typedef delegates::CDelegate1<IObject*&> Delegate;
		/** Register delegate function that creates object for specified _category and _type. */
		void registerFactory(const std::string& _category, const std::string& _type, Delegate::IDelegate* _delegate);
		/** Unregister delegate function that creates object for specified _category and _type. */
		void unregisterFactory(const std::string& _category, const std::string& _type);
		/** Unregister all delegate functions that creates object for specified _category. */
		void unregisterFactory(const std::string& _category);

		/** Is factory for specified _category and _type exist. */
		bool isFactoryExist(const std::string& _category, const std::string& _type);

		/** Register factory for specified _category. */
		template<typename Type>
		void registerFactory(const std::string& _category)
		{
			registerFactory(_category, Type::getClassTypeName(), GenericFactory<Type>::getFactory());
		}

		/** Register factory for specified _category and _type. */
		template<typename Type>
		void registerFactory(const std::string& _category, const std::string& _type)
		{
			registerFactory(_category, _type, GenericFactory<Type>::getFactory());
		}

		/** Unegister factory for specified _category. */
		template<typename Type>
		void unregisterFactory(const std::string& _category)
		{
			unregisterFactory(_category, Type::getClassTypeName());
		}

		/** Create object with given _category and _type. Return nullptr if there's no registered factory. */
		IObject* createObject(const std::string& _category, const std::string& _type);
		/** Create object with given _category and _type. Return nullptr if there's no registered factory. */
		template<typename Type>
		Type* createObject(const std::string& _category)
		{
			IObject* item = createObject(_category, Type::getClassTypeName());
			if (item != nullptr)
				return item->castType<Type>(false);
			return nullptr;
		}

		/** Destroy object */
		void destroyObject(IObject* _object);

	private:
		typedef std::map<std::string, Delegate> MapFactoryItem;
		typedef std::map<std::string, MapFactoryItem> MapRegisterFactoryItem;
		MapRegisterFactoryItem mRegisterFactoryItems;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_FACTORY_MANAGER_H__
