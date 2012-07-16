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
	class DataInfoManager
	{
	public:
		DataInfoManager();
		~DataInfoManager();

		static DataInfoManager& getInstance();
		static DataInfoManager* getInstancePtr();

		void initialise();
		void shutdown();

		void load(const std::string& _fileName);
		void clear();

		DataInfo* getData(const std::string& _type);

	private:
		static DataInfoManager* mInstance;
		typedef std::vector<DataInfo*> VectorDataInfo;
		VectorDataInfo mDataInfos;
	};
}

#endif
