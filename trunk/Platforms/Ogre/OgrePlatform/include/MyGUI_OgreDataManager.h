/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef __MYGUI_OGRE_DATA_MANAGER_H__
#define __MYGUI_OGRE_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OgreDataManager :
		public DataManager,
		public Singleton<OgreDataManager>
	{
	public:
		OgreDataManager();

		void initialise(const std::string& _group);
		void shutdown();

		static OgreDataManager& getInstance() { return Singleton<OgreDataManager>::getInstance(); }
		static OgreDataManager* getInstancePtr() { return Singleton<OgreDataManager>::getInstancePtr(); }

		const std::string& getGroup() { return mGroup; }

		/** @see DataManager::getData */
		virtual IDataStream* getData(const std::string& _name);

		typedef std::vector<std::string> VectorString;

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

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_DATA_MANAGER_H__
