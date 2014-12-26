/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#ifndef MYGUI_DUMMY_DATA_MANAGER_H_
#define MYGUI_DUMMY_DATA_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class DummyDataManager :
		public DataManager
	{
	public:
		DummyDataManager();

		void initialise();
		void shutdown();

		static DummyDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static DummyDataManager* getInstancePtr()
		{
			return static_cast<DummyDataManager*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::freeData */
		virtual void freeData(IDataStream* _data);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);
	};

} // namespace MyGUI

#endif // MYGUI_DUMMY_DATA_MANAGER_H_
