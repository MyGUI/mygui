/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef __MYGUI_OPENGL_DATA_MANAGER_H__
#define __MYGUI_OPENGL_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OpenGLDataManager :
		public DataManager
	{
	public:
		OpenGLDataManager();

		void initialise();
		void shutdown();

		static OpenGLDataManager& getInstance() { return static_cast<OpenGLDataManager&>(Singleton<DataManager>::getInstance()); }
		static OpenGLDataManager* getInstancePtr() { return static_cast<OpenGLDataManager*>(Singleton<DataManager>::getInstancePtr()); }

		/** @see DataManager::getData */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath */
		const std::string& getDataPath(const std::string& _name);

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

#endif // __MYGUI_OPENGL_DATA_MANAGER_H__
