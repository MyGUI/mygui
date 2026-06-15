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
		static bool isUniqueName(DataPtr _data, std::string_view _propertyName);
		static void executeAction(PropertyPtr _property, std::string_view _value, bool _merge = false);

		using PairProprty = std::pair<PropertyPtr, std::string>;
		using VectorPairProperty = std::vector<PairProprty>;
		static void storeUniqueNameProperty(
			std::string_view _propertyName,
			std::string_view _propertyUnique,
			DataPtr _parent,
			VectorPairProperty& _store);
		static void restoreUniqueNameProperty(VectorPairProperty& _store);

		static PropertyPtr getPropertyByName(std::string_view _dataType, std::string_view _propertyName);
		static bool isDataSelected(DataPtr _data);

	private:
		static PropertyPtr getPropertyByName(DataPtr _data, std::string_view _dataType, std::string_view _propertyName);
		static bool isDataSelected(DataPtr _parent, DataPtr _data);
	};
}

#endif
