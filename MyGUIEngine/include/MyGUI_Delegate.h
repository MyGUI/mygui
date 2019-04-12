/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DELEGATE_H_
#define MYGUI_DELEGATE_H_

#include "MyGUI_Diagnostic.h"
#include "MyGUI_Any.h"
#include <list>

#include <functional>

namespace MyGUI
{

namespace delegates
{
	// base class for unsubscribing from multi delegates
	class IDelegateUnlink
	{
	public:
		virtual ~IDelegateUnlink() = default;

		IDelegateUnlink()
		{
			m_baseDelegateUnlink = this;
		}
		bool compare(IDelegateUnlink* _unlink) const
		{
			return m_baseDelegateUnlink == _unlink->m_baseDelegateUnlink;
		}

	private:
		IDelegateUnlink* m_baseDelegateUnlink;
	};

	template <typename ...Args>
	class DelegateFunction
	{
	public:
		using Function = std::function<void(Args...)>;

		// function or static class method
		DelegateFunction(Function _function, Any _functionPointer) :
			mFunction(_function),
			mFunctionPointer(_functionPointer)
		{
		}

		// non-static class method
		DelegateFunction(Function _function, Any _functionPointer, IDelegateUnlink* _object) :
			mFunction(_function),
			mUnlink(_object),
			mObject(_object),
			mFunctionPointer(_functionPointer)
		{
		}

		// non-static class method
		DelegateFunction(Function _function, Any _functionPointer, void* _object) :
			mFunction(_function),
			mUnlink(nullptr),
			mObject(_object),
			mFunctionPointer(_functionPointer)
		{
		}

		void invoke(Args... args)
		{
			mFunction(args...);
		}

		bool compare(DelegateFunction<Args...>* _delegate) const
		{
			if (nullptr == _delegate) return false;
			return _delegate->mObject == mObject && _delegate->mFunctionPointer.compare(mFunctionPointer);
		}

		bool compare(IDelegateUnlink* _unlink) const
		{
			return mUnlink == _unlink;
		}

	private:
		Function mFunction;

		IDelegateUnlink* mUnlink = nullptr;
		void* mObject = nullptr;
		Any mFunctionPointer;
	};

} // namespace delegates

// Creates delegate from a function or a static class method
template <typename ...Args>
inline delegates::DelegateFunction<Args...>* newDelegate(void(*_func)(Args... args))
{
	return new delegates::DelegateFunction<Args...>(_func, _func);
}

// Creates delegate from a non-static class method
template <typename T, typename ...Args>
inline delegates::DelegateFunction<Args...>* newDelegate(T* _object, void (T::*_method)(Args... args))
{
	return new delegates::DelegateFunction<Args...>(
		[=](Args&&... args) { return (_object->*_method)(std::forward<decltype(args)>(args)...); },
		_method,
		_object);
}

namespace delegates
{

	template <typename ...Args>
	class Delegate
	{
	public:
		using IDelegate = DelegateFunction<Args...>;

		Delegate() : mDelegate(nullptr) { }
		Delegate(const Delegate& _event) : mDelegate(nullptr)
		{
			// take ownership
			mDelegate = _event.mDelegate;
			const_cast<Delegate&>(_event).mDelegate = nullptr;
		}

		~Delegate()
		{
			clear();
		}

		bool empty() const
		{
			return mDelegate == nullptr;
		}

		void clear()
		{
			delete mDelegate;
			mDelegate = nullptr;
		}

		Delegate& operator=(IDelegate* _delegate)
		{
			delete mDelegate;
			mDelegate = _delegate;
			return *this;
		}

		Delegate& operator=(const Delegate<Args...>& _event)
		{
			if (this == &_event)
				return *this;

			// take ownership
			IDelegate* del = _event.mDelegate;
			const_cast<Delegate&>(_event).mDelegate = nullptr;

			if (mDelegate != nullptr && !mDelegate->compare(del))
				delete mDelegate;

			mDelegate = del;

			return *this;
		}

		void operator()(Args... args)
		{
			if (mDelegate == nullptr) return;
			mDelegate->invoke(args...);
		}

	private:
		IDelegate* mDelegate;
	};

	template <typename ...Args>
	class MultiDelegate
	{
	public:
		using IDelegate = DelegateFunction<Args...>;
		using ListDelegate = typename std::list<IDelegate*>;

		MultiDelegate() { }
		~MultiDelegate()
		{
			clear();
		}

		bool empty() const
		{
			for (const auto& delegate : mListDelegates)
			{
				if (delegate) return false;
			}
			return true;
		}

		void clear()
		{
			for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}

		void clear(IDelegateUnlink* _unlink)
		{
			for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_unlink))
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}

		void operator+=(IDelegate* _delegate)
		{
			for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					MYGUI_EXCEPT("Trying to add same delegate twice.");
				}
			}
			mListDelegates.push_back(_delegate);
		}

		void operator-=(IDelegate* _delegate)
		{
			for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					if ((*iter) != _delegate) delete (*iter);
					(*iter) = nullptr;
					break;
				}
			}
			delete _delegate;
		}

		void operator()(Args... args)
		{
			auto iter = mListDelegates.begin();
			while (iter != mListDelegates.end())
			{
				if (nullptr == (*iter))
				{
					iter = mListDelegates.erase(iter);
				}
				else
				{
					(*iter)->invoke(args...);
					++iter;
				}
			}
		}

		MultiDelegate(const MultiDelegate& _event)
		{
			// take ownership
			ListDelegate del = _event.mListDelegates;
			const_cast<MultiDelegate&>(_event).mListDelegates.clear();

			safe_clear(del);

			mListDelegates = del;
		}

		MultiDelegate& operator=(const MultiDelegate& _event)
		{
			// take ownership
			ListDelegate del = _event.mListDelegates;
			const_cast<MultiDelegate&>(_event).mListDelegates.clear();

			safe_clear(del);

			mListDelegates = del;

			return *this;
		}

		MYGUI_OBSOLETE("use : operator += ")
		MultiDelegate& operator=(IDelegate* _delegate)
		{
			clear();
			*this += _delegate;
			return *this;
		}

	private:
		void safe_clear(ListDelegate& _delegates)
		{
			for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					IDelegate* del = (*iter);
					(*iter) = nullptr;
					delete_is_not_found(del, _delegates);
				}
			}
		}

		void delete_is_not_found(IDelegate* _del, ListDelegate& _delegates)
		{
			for (auto iter = _delegates.begin(); iter != _delegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_del))
				{
					return;
				}
			}

			delete _del;
		}

	private:
		ListDelegate mListDelegates;
	};

//#ifndef MYGUI_DONT_USE_OBSOLETE // TODO
	using CDelegate0 = Delegate<>;
	template <typename ...Args>
	using CDelegate1 = Delegate<Args...>;
	template <typename ...Args>
	using CDelegate2 = Delegate<Args...>;
	template <typename ...Args>
	using CDelegate3 = Delegate<Args...>;
	template <typename ...Args>
	using CDelegate4 = Delegate<Args...>;
	template <typename ...Args>
	using CDelegate5 = Delegate<Args...>;
	template <typename ...Args>
	using CDelegate6 = Delegate<Args...>;

	using CMultiDelegate0 = MultiDelegate<>;
	template <typename ...Args>
	using CMultiDelegate1 = MultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate2 = MultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate3 = MultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate4 = MultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate5 = MultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate6 = MultiDelegate<Args...>;
//#endif
}

} // namespace MyGUI

#endif // MYGUI_DELEGATE_H_
