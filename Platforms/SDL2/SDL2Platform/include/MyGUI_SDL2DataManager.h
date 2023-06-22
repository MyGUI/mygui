/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#ifndef MYGUI_SDL2_DATA_MANAGER_H_
#define MYGUI_SDL2_DATA_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

#include <string>
#include <vector>

namespace MyGUI
{

	class SDL2DataManager :
		public DataManager
	{
	public:
		SDL2DataManager();

		void initialise();
		void shutdown();

		static SDL2DataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static SDL2DataManager* getInstancePtr()
		{
			return static_cast<SDL2DataManager*>(DataManager::getInstancePtr());
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

		/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // MYGUI_SDL2_DATA_MANAGER_H_
