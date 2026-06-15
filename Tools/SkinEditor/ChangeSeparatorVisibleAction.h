/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _24534531_49fc_4c89_8389_7dbc5b17de06_
#define _24534531_49fc_4c89_8389_7dbc5b17de06_

#include "ActionChangeDataProperty.h"
#include "Data.h"

namespace tools
{

	class ChangeSeparatorVisibleAction : public ActionChangeDataProperty
	{
	public:
		void doAction() override;
		void undoAction() override;

	private:
		using PairProprty = std::pair<PropertyPtr, std::string>;
		using VectorPairProperty = std::vector<PairProprty>;
		void storeRegionValues(DataPtr _skinData, VectorPairProperty& _store);

		void storeOldValues();
		void setNewValues();

	private:
		VectorPairProperty mOldValues;
	};

}

#endif
