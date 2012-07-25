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

	class ChangeSeparatorVisibleAction :
		public ActionChangeDataProperty
	{
	public:
		ChangeSeparatorVisibleAction();
		virtual ~ChangeSeparatorVisibleAction();

		virtual void doAction();
		virtual void undoAction();

	private:
		typedef std::pair<PropertyPtr, std::string> PairProprty;
		typedef std::vector<PairProprty> VectorPairProperty;
		void storeRegionValues(DataPtr _skinData, VectorPairProperty& _store);

		void storeOldValues();
		void setNewValues();

	private:
		VectorPairProperty mOldValues;
	};

}

#endif
