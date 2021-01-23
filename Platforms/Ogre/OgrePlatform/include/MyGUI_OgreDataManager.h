/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef MYGUI_OGRE_DATA_MANAGER_H_
#define MYGUI_OGRE_DATA_MANAGER_H_

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
		IDataStream* getData(const std::string& _name) const override;

		/** @see DataManager::freeData */
		void freeData(IDataStream* _data) override;

		/** @see DataManager::isDataExist */
		bool isDataExist(const std::string& _name) const override;

		/** @see DataManager::getDataListNames */
		const VectorString& getDataListNames(const std::string& _pattern) const override;

		/** @see DataManager::getDataPath */
		const std::string& getDataPath(const std::string& _name) const override;

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		const VectorString& getDataListNames(const std::string& _pattern, bool _fullpath) const;

	private:
		std::string mGroup;
		bool mAllGroups;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_DATA_MANAGER_H_
