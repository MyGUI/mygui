/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _992be787_40d0_4bde_8e4c_db4ba7a18854_
#define _992be787_40d0_4bde_8e4c_db4ba7a18854_

#include "Action.h"
#include <string>
#include "Data.h"

namespace tools
{
	class ActionCreateData :
		public Action
	{
	public:
		ActionCreateData();
		virtual ~ActionCreateData();

		virtual void doAction();
		virtual void undoAction();

		void setName(const std::string& _value);
		void setParent(Data* _parent);

	private:
		std::string mName;
		Data* mData;
		Data* mParent;
		bool mComplete;
	};
}

#endif
