/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _eb5f2935_ea69_4254_a22e_d2f73c8e9f1c_
#define _eb5f2935_ea69_4254_a22e_d2f73c8e9f1c_

#include "Action.h"
#include "Data.h"

namespace tools
{
	class ActionSwapData :
		public Action
	{
	public:
		ActionSwapData();
		virtual ~ActionSwapData();

		virtual void doAction();
		virtual void undoAction();

		void setData1(Data* _data);
		void setData2(Data* _data);

	private:
		Data* mData1;
		Data* mData2;
		size_t mIndex;
	};
}

#endif
