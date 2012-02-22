/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef __MYGUI_OGRE_DATA_MANAGER_H__
#define __MYGUI_OGRE_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OgreDataManager :
		public DataManager
	{
	public:
		OgreDataManager();

		void initialise(const std::string& _group);
		void shutdown();

		static OgreDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static OgreDataManager* getInstancePtr()
		{
			return static_cast<OgreDataManager*>(DataManager::getInstancePtr());
		}

		const std::string& getGroup() const
		{
			return mGroup;
		}

		/** @see DataManager::getData */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath */
		virtual const std::string& getDataPath(const std::string& _name);

	private:
		const VectorString& getDataListNames(const std::string& _pattern, bool _fullpath);

	private:
		std::string mGroup;
		bool mAllGroups;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_DATA_MANAGER_H__
