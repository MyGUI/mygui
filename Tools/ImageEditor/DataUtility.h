/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _ca4d715a_a85f_4bd1_b3f2_4181f9648d87_
#define _ca4d715a_a85f_4bd1_b3f2_4181f9648d87_

#include "DataManager.h"

namespace tools
{
	class DataUtility
	{
	public:
		static Data* getSelectedDataByType(const std::string& _type);
		static Data* getSelectedParentDataByType(const std::string& _type);

		static void cloneData(Data* _target, Data* _prototype);
		static std::string getUniqueName(Data* _parent, const std::string& _pattern);

	private:
		static Data* getSelectedDataByType(Data* _data, DataType* _info);
		static Data* getSelectedParentDataByType(Data* _data, DataType* _info);
		static void copyProperty(Data* _target, Data* _prototype);
		static bool checkUniqueName(Data* _parent, const std::string& _name);
	};
}

#endif
