/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _ccf347aa_fadc_4b79_8b3b_73475259614d_
#define _ccf347aa_fadc_4b79_8b3b_73475259614d_

#include <string>
#include <vector>
#include "DataInfo.h"

namespace tools
{
	class DataManager
	{
	public:
		DataManager();
		~DataManager();

		static DataManager& getInstance();
		static DataManager* getInstancePtr();

		void loadDataInfo(const std::string& _fileName);
		void clearDataInfo();

		DataInfo* getData(const std::string& _type);

	private:
		static DataManager* mInstance;
		typedef std::vector<DataInfo*> VectorDataInfo;
		VectorDataInfo mDataInfos;
	};
}

#endif
