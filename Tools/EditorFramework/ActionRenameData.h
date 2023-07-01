/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _2f51a91d_af38_483c_81db_bda0b2d01f24_
#define _2f51a91d_af38_483c_81db_bda0b2d01f24_

#include "ActionChangeDataProperty.h"
#include "Data.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ActionRenameData : public ActionChangeDataProperty
	{
	public:
		void doAction() override;
		void undoAction() override;

	private:
		using PairProprty = std::pair<PropertyPtr, std::string>;
		using VectorPairProperty = std::vector<PairProprty>;
		VectorPairProperty mOldValues;
	};

}

#endif
