/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _a385b907_0dd3_4832_8445_fffb8f734d39_
#define _a385b907_0dd3_4832_8445_fffb8f734d39_

#include "Action.h"
#include <string>
#include "Data.h"

namespace tools
{

	class CreateSkinDataAction : public Action
	{
	public:
		CreateSkinDataAction();

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
