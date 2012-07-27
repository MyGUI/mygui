/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "Action.h"

namespace tools
{
	Action::Action() :
		mMerge(false)
	{
	}

	Action::~Action()
	{
	}

	void Action::doAction()
	{
	}

	void Action::undoAction()
	{
	}

	bool Action::doMerge(Action* _action)
	{
		return false;
	}

	void Action::setMerge(bool _value)
	{
		mMerge = _value;
	}

	bool Action::getMerge() const
	{
		return mMerge;
	}
}
