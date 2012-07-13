/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _6463bc0c_937b_472d_9c7c_27746cb7d448_
#define _6463bc0c_937b_472d_9c7c_27746cb7d448_

#include "Action.h"
#include "Data.h"

namespace tools
{
	class ActionRenameData :
		public Action
	{
	public:
		ActionRenameData();
		virtual ~ActionRenameData();

		virtual void doAction();
		virtual void undoAction();

		void setData(Data* _data);
		void setName(const std::string& _value);

	private:
		std::string mName;
		std::string mOldName;
		Data* mData;
	};
}

#endif
