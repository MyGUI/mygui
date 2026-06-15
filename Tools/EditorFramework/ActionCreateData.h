/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _992be787_40d0_4bde_8e4c_db4ba7a18854_
#define _992be787_40d0_4bde_8e4c_db4ba7a18854_

#include "Action.h"
#include <string>
#include "Data.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ActionCreateData : public Action
	{
	public:
		ActionCreateData();

		void doAction() override;
		void undoAction() override;

		void setParent(DataPtr _parent);
		void setType(std::string_view _value);
		void setUniqueProperty(std::string_view _value);

	private:
		std::string mType;
		DataPtr mData;
		DataPtr mParent;
		std::string mUniqueProperty;

		using PairProprty = std::pair<PropertyPtr, std::string>;
		using VectorPairProperty = std::vector<PairProprty>;
		VectorPairProperty mOldValues;
	};

}

#endif
