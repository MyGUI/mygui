/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _c7757d42_4060_44df_a0c2_06275945b83f_
#define _c7757d42_4060_44df_a0c2_06275945b83f_

#include "Action.h"
#include "Data.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ActionCloneData :
		public Action
	{
	public:
		ActionCloneData();
		virtual ~ActionCloneData();

		virtual void doAction();
		virtual void undoAction();

		void setPrototype(DataPtr _prototype);
		void setUniqueProperty(const std::string& _value);

	private:
		std::string mType;
		DataPtr mData;
		DataPtr mParent;
		DataPtr mPrototype;
		std::string mUniqueProperty;

		typedef std::pair<PropertyPtr, std::string> PairProprty;
		typedef std::vector<PairProprty> VectorPairProperty;
		VectorPairProperty mOldValues;
	};

}

#endif
