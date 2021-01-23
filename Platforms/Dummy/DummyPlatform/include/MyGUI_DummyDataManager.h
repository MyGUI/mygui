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
		IDataStream* getData(const std::string& _name) const override;

		/** @see DataManager::freeData */
		void freeData(IDataStream* _data) override;

		/** @see DataManager::isDataExist(const std::string& _name) */
		bool isDataExist(const std::string& _name) const override;

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		const VectorString& getDataListNames(const std::string& _pattern) const override;

		/** @see DataManager::getDataPath(const std::string& _name) */
		const std::string& getDataPath(const std::string& _name) const override;
	};

} // namespace MyGUI

#endif // MYGUI_DUMMY_DATA_MANAGER_H_
