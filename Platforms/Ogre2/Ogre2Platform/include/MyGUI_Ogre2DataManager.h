/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef MYGUI_OGRE2_DATA_MANAGER_H_
#define MYGUI_OGRE2_DATA_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class Ogre2DataManager :
		public DataManager
	{
	public:
		Ogre2DataManager();

		void initialise(const std::string& _group);
		void shutdown();

		static Ogre2DataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static Ogre2DataManager* getInstancePtr()
		{
			return static_cast<Ogre2DataManager*>(DataManager::getInstancePtr());
		}

		const std::string& getGroup() const
		{
			return mGroup;
		}

		/** @see DataManager::getData */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::freeData */
		virtual void freeData(IDataStream* _data);

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

#endif // MYGUI_OGRE_DATA_MANAGER_H_
