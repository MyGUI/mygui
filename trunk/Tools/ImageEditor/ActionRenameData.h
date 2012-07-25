/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _6463bc0c_937b_472d_9c7c_27746cb7d448_
#define _6463bc0c_937b_472d_9c7c_27746cb7d448_

#include "ActionChangeDataProperty.h"
#include "Data.h"

namespace tools
{

	class ActionRenameData :
		public ActionChangeDataProperty
	{
	public:
		ActionRenameData();
		virtual ~ActionRenameData();

		virtual void doAction();
		virtual void undoAction();

	private:
		typedef std::pair<PropertyPtr, std::string> PairProprty;
		typedef std::vector<PairProprty> VectorPairProperty;
		VectorPairProperty mOldValues;
	};

}

#endif
