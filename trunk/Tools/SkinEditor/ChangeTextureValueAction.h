/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _2b332f19_cb04_45f3_84dd_ff54d9723078_
#define _2b332f19_cb04_45f3_84dd_ff54d9723078_

#include "ActionChangeDataProperty.h"
#include "Data.h"

namespace tools
{

	class ChangeTextureValueAction :
		public ActionChangeDataProperty
	{
	public:
		ChangeTextureValueAction();
		virtual ~ChangeTextureValueAction();

		virtual void doAction();
		virtual void undoAction();
		virtual bool doMerge(Action* _action);

	private:
		std::string mOldValue;
	};

}

#endif
