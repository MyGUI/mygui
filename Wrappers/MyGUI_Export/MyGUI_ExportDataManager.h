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
		IDataStream* getData(const std::string& _name) const override;

		/** @see DataManager::freeData */
		void freeData(IDataStream* _data) override;

		/** @see DataManager::isDataExist(const std::string& _name) */
		bool isDataExist(const std::string& _name) const override;

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		const VectorString& getDataListNames(const std::string& _pattern) const override;

		/** @see DataManager::getDataPath(const std::string& _name) */
		std::string getDataPath(const std::string& _name) const override;

	/*internal:*/
		void addResourceLocation(const std::string& _path, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		using VectorArhivInfo = std::vector<ArhivInfo>;
		VectorArhivInfo mPaths;
	};

}

#endif
