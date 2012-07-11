/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _d92392a8_2d98_4639_8dfe_7cb945c817b5_
#define _d92392a8_2d98_4639_8dfe_7cb945c817b5_

#include "Command.h"
#include "Data.h"

namespace tools
{
	class CommandDestroyData :
		public Command
	{
	public:
		CommandDestroyData();
		virtual ~CommandDestroyData();

		virtual void doCommand();
		virtual void undoCommand();

		void setData(Data* _data);

	private:
		Data* mData;
	};
}

#endif
