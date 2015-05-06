/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL3_DATA_MANAGER_H_
#define MYGUI_OPENGL3_DATA_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OpenGL3DataManager :
		public DataManager
	{
	public:
		OpenGL3DataManager();

		void initialise();
		void shutdown();

		static OpenGL3DataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static OpenGL3DataManager* getInstancePtr()
		{
			return static_cast<OpenGL3DataManager*>(DataManager::getInstancePtr());
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

#endif // MYGUI_OPENGL3_DATA_MANAGER_H_
