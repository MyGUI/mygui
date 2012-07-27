/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _0228cbb9_aa86_46b7_b23f_3d6ee87da9f9_
#define _0228cbb9_aa86_46b7_b23f_3d6ee87da9f9_

#include "Data.h"
#include "Property.h"

namespace tools
{
	class MYGUI_EXPORT_DLL PropertyUtility
	{
	public:
		static bool isUniqueName(DataPtr _data, const std::string& _propertyName);
		static void executeAction(PropertyPtr _property, const std::string& _value, bool _merge = false);

		typedef std::pair<PropertyPtr, std::string> PairProprty;
		typedef std::vector<PairProprty> VectorPairProperty;
		static void storeUniqueNameProperty(const std::string& _propertyName, const std::string& _propertyUnique, DataPtr _parent, VectorPairProperty& _store);
		static void restoreUniqueNameProperty(VectorPairProperty& _store);

		static PropertyPtr getPropertyByName(const std::string& _dataType, const std::string& _propertyName);
		static bool isDataSelected(DataPtr _data);

	private:
		static PropertyPtr getPropertyByName(DataPtr _data, const std::string& _dataType, const std::string& _propertyName);
		static bool isDataSelected(DataPtr _parent, DataPtr _data);
	};
}

#endif
