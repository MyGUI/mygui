/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _ccf347aa_fadc_4b79_8b3b_73475259614d_
#define _ccf347aa_fadc_4b79_8b3b_73475259614d_

#include <string>
#include <vector>
#include "DataType.h"

namespace tools
{

	class MYGUI_EXPORT_DLL DataTypeManager
	{
	public:
		DataTypeManager();
		~DataTypeManager();

		static DataTypeManager& getInstance();
		static DataTypeManager* getInstancePtr();

		void initialise();
		void shutdown();

		void load(const std::string& _fileName);
		void clear();

		DataTypePtr getType(std::string_view _type);
		DataTypePtr getParentType(std::string_view _type);

	private:
		static DataTypeManager* mInstance;
		using VectorDataInfo = std::vector<DataTypePtr>;
		VectorDataInfo mDataInfos;
	};

}

#endif
