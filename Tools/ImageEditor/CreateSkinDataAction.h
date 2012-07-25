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

	class CreateSkinDataAction :
		public Action
	{
	public:
		CreateSkinDataAction();
		virtual ~CreateSkinDataAction();

		virtual void doAction();
		virtual void undoAction();

		void setParent(DataPtr _parent);
		void setType(const std::string& _value);
		void setUniqueProperty(const std::string& _value);

	private:
		std::string mType;
		DataPtr mData;
		DataPtr mParent;
		std::string mUniqueProperty;

		typedef std::pair<PropertyPtr, std::string> PairProprty;
		typedef std::vector<PairProprty> VectorPairProperty;
		VectorPairProperty mOldValues;
	};

}

#endif
