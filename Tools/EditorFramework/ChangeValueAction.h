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

	class MYGUI_EXPORT_DLL ChangeValueAction :
		public ActionChangeDataProperty
	{
	public:
		ChangeValueAction();
		virtual ~ChangeValueAction();

		virtual void doAction();
		virtual void undoAction();
		virtual bool doMerge(Action* _action);

	private:
		std::string mOldValue;
	};

}

#endif
