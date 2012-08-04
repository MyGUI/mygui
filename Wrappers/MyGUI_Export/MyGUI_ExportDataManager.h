/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef __MYGUI_EXPORT_DATA_MANAGER_H__
#define __MYGUI_EXPORT_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class ExportDataManager :
		public DataManager
	{
	public:
		ExportDataManager();

		void initialise();
		void shutdown();

		static ExportDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static ExportDataManager* getInstancePtr()
		{
			return static_cast<ExportDataManager*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::destroyData */
		virtual void destroyData(IDataStream* _data);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

	private:
		typedef std::pair<MyGUI::IDataStream*, size_t> DataCounter;
		typedef std::map<std::string, DataCounter> MapData;
		MapData mDatas;
	};

} // namespace MyGUI

#endif // __MYGUI_EXPORT_DATA_MANAGER_H__
