/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _e5216f11_26e7_4658_a14b_ecd7c54a3207_
#define _e5216f11_26e7_4658_a14b_ecd7c54a3207_

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

		static ExportDataManager& getInstance();
		static ExportDataManager* getInstancePtr();

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
		void addResourceLocation(const std::string& _path, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;
	};

}

#endif
