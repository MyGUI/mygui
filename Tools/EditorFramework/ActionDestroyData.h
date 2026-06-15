/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _d92392a8_2d98_4639_8dfe_7cb945c817b5_
#define _d92392a8_2d98_4639_8dfe_7cb945c817b5_

#include "Action.h"
#include "Data.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ActionDestroyData : public Action
	{
	public:
		void doAction() override;
		void undoAction() override;

		void setData(DataPtr _data);
		void setUniqueProperty(std::string_view _value);

	private:
		DataPtr mData{nullptr};
		DataPtr mParent{nullptr};
		size_t mIndex{MyGUI::ITEM_NONE};
		std::string mUniqueProperty;

		using PairProprty = std::pair<PropertyPtr, std::string>;
		using VectorPairProperty = std::vector<PairProprty>;
		VectorPairProperty mOldValues;
	};

}

#endif
