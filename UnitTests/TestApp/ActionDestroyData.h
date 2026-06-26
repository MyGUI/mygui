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
	class ActionDestroyData : public Action
	{
	public:
		ActionDestroyData();
		~ActionDestroyData() override;

		void doAction() override;
		void undoAction() override;

		void setData(Data* _data);

	private:
		Data* mData;
		bool mComplete;
	};
}

#endif
