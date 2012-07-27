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

	class MYGUI_EXPORT_DLL ActionChangePositionData :
		public Action
	{
	public:
		ActionChangePositionData();
		virtual ~ActionChangePositionData();

		virtual void doAction();
		virtual void undoAction();

		void setData1(DataPtr _data);
		void setData2(DataPtr _data);

	private:
		DataPtr mData1;
		DataPtr mData2;
		size_t mIndex;
	};

}

#endif
