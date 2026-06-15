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

	class MYGUI_EXPORT_DLL DataUtility
	{
	public:
		static DataPtr getSelectedDataByType(std::string_view _type);
		static DataPtr getSelectedParentDataByType(std::string_view _type);

		static void cloneData(DataPtr _target, DataPtr _prototype);
		static std::string getUniqueName(DataPtr _parent, std::string_view _pattern);

		static Data::VectorData getChildsByType(DataPtr _parent, std::string_view _type, bool _friend = true);

	private:
		static DataPtr getSelectedDataByType(DataPtr _data, DataTypePtr _info);
		static DataPtr getSelectedParentDataByType(DataPtr _data, DataTypePtr _info);
		static void copyProperty(DataPtr _target, DataPtr _prototype);
		static bool checkUniqueName(DataPtr _parent, std::string_view _name);
	};

}

#endif
