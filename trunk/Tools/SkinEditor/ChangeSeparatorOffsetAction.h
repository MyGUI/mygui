/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _90c48c0f_69d2_4db8_8df3_663e6f8d34cb_
#define _90c48c0f_69d2_4db8_8df3_663e6f8d34cb_

#include "ActionChangeDataProperty.h"
#include "Data.h"

namespace tools
{

	class ChangeSeparatorOffsetAction :
		public ActionChangeDataProperty
	{
	public:
		ChangeSeparatorOffsetAction();
		virtual ~ChangeSeparatorOffsetAction();

		virtual void doAction();
		virtual void undoAction();
		virtual bool doMerge(Action* _action);

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
