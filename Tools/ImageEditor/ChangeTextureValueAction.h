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

	class ChangeTextureValueAction : public ActionChangeDataProperty
	{
	public:
		void doAction() override;
		void undoAction() override;
		bool doMerge(Action* _action) override;

	private:
		std::string mOldValue;
	};

}

#endif
