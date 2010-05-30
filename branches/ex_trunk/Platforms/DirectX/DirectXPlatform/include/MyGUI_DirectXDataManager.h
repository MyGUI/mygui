/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
	@module
*/

#ifndef __MYGUI_OGRE_DATA_MANAGER_H__
#define __MYGUI_OGRE_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class DirectXDataManager :
		public DataManager,
		public Singleton<DirectXDataManager>
	{
	public:
		void initialise();
		void shutdown();

		static DirectXDataManager& getInstance() { return Singleton<DirectXDataManager>::getInstance(); }
		static DirectXDataManager* getInstancePtr() { return Singleton<DirectXDataManager>::getInstancePtr(); }

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::string name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_DATA_MANAGER_H__
