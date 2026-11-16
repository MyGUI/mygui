/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Delegate.h"
#include <algorithm>

namespace MyGUI::delegates::detail
{

	DelegateBase::~DelegateBase()
	{
		mDestroying = true;
		clear();
	}

	void DelegateBase::clear()
	{
		if (auto* old = std::exchange(mDelegate, nullptr))
			old->release();
	}

	DelegateBase& DelegateBase::operator=(DelegateFunctionBase* _delegate)
	{
		if (mDestroying)
		{
			if (_delegate)
				_delegate->release();
			return *this;
		}
		if (mDelegate != _delegate)
		{
			if (auto* old = std::exchange(mDelegate, _delegate))
				old->release();
		}
		return *this;
	}

	MultiDelegateBase::~MultiDelegateBase()
	{
		mDestroying = true;
		if (auto* state = std::exchange(mState, nullptr))
		{
			Operation guard(state);
			state->closed = true;
			state->clear();
			--state->references; // Release the event's ownership; the guard retains state.
		}
	}

	void MultiDelegateBase::clear()
	{
		if (auto* state = mState)
		{
			Operation guard(state);
			state->clear();
		}
	}

	void MultiDelegateBase::clear(IDelegateUnlink* _unlink)
	{
		if (auto* state = mState; state && _unlink)
		{
			Operation guard(state);
			for (size_t i = 0; i < state->callbacks.size(); ++i)
			{
				auto* callback = state->callbacks[i];
				if (callback && callback->compare(_unlink))
					state->remove(i);
			}
		}
	}

	void MultiDelegateBase::operator+=(DelegateFunctionBase* _delegate)
	{
		if (!_delegate)
			return;
		if (mDestroying)
		{
			delete _delegate;
			return;
		}
		if (!mState)
			mState = new State;
		auto* state = mState;
		Operation guard(state);
		state->add(_delegate);
	}

	void MultiDelegateBase::operator-=(DelegateFunctionBase* _delegate)
	{
		if (!_delegate)
			return;
		if (auto* state = mState)
		{
			Operation guard(state);
			// The original pointer can also be supplied; never delete an owned or retired callback here.
			std::unique_ptr<DelegateFunctionBase> token(_delegate->mOwner ? nullptr : _delegate);
			const auto index = state->find(_delegate);
			if (index != state->callbacks.size())
				state->remove(index);
		}
		else if (!_delegate->mOwner)
			delete _delegate;
	}

	MultiDelegateBase& MultiDelegateBase::operator=(DelegateFunctionBase* _delegate)
	{
		if (mDestroying)
		{
			delete _delegate;
			return *this;
		}
		if (!mState)
		{
			*this += _delegate;
			return *this;
		}
		auto* state = mState;
		Operation guard(state);
		state->clear();
		state->collectIfIdle();
		// Releasing old captures can destroy this event. From here on, use only retained state.
		state->add(_delegate);
		return *this;
	}

	size_t MultiDelegateBase::State::find(DelegateFunctionBase* _delegate) const
	{
		for (size_t i = 0; i < callbacks.size(); ++i)
		{
			auto* callback = callbacks[i];
			// Custom Any identities can execute user comparison code and reenter the event.
			if (callback && callback->compare(_delegate) && callbacks[i] == callback)
				return i;
		}
		return callbacks.size();
	}

	void MultiDelegateBase::State::add(DelegateFunctionBase* _delegate)
	{
		if (!_delegate)
			return;
		if (closed)
		{
			delete _delegate;
			return;
		}
		const auto index = find(_delegate);
		if (_delegate->mOwner || index != callbacks.size())
			MYGUI_EXCEPT("Trying to add same delegate twice.");
		if (closed)
		{
			delete _delegate;
			return;
		}
		callbacks.push_back(_delegate); // Ownership transfers only after successful insertion.
		extent = callbacks.size();
		_delegate->mOwner = this;
		++active;
	}

	void MultiDelegateBase::State::remove(size_t _index)
	{
		auto* callback = std::exchange(callbacks[_index], nullptr);
		--active;
		if (retiredTail)
			retiredTail->mNextRetired = callback;
		else
			retired = callback;
		retiredTail = callback;
	}

	void MultiDelegateBase::State::clear()
	{
		for (size_t i = 0; i < callbacks.size(); ++i)
		{
			if (callbacks[i])
				remove(i);
		}
	}

	void MultiDelegateBase::State::collectIfIdle()
	{
		while (retired && references == (closed ? 1u : 2u))
		{
			callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), nullptr), callbacks.end());
			extent = callbacks.size();
			auto* removed = std::exchange(retired, nullptr);
			retiredTail = nullptr;
			// Detach the whole batch before running capture destructors. Drain any reentrant removals too.
			while (removed)
			{
				auto* next = std::exchange(removed->mNextRetired, nullptr);
				// Keep ownership marked while capture destructors can remove this original pointer again.
				removed->release();
				removed = next;
			}
		}
	}

}
