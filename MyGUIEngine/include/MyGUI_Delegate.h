/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DELEGATE_H_
#define MYGUI_DELEGATE_H_

#include "MyGUI_Diagnostic.h"
#include <list>

#include <typeinfo>

namespace MyGUI
{

namespace delegates
{
	// base class for unsubscribing from multi delegates
	class MYGUI_EXPORT IDelegateUnlink
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

	inline IDelegateUnlink* GetDelegateUnlink(void* _base)
	{
		return nullptr;
	}
	inline IDelegateUnlink* GetDelegateUnlink(IDelegateUnlink* _base)
	{
		return _base;
	}

	template <typename ...Args>
	class IDelegate
	{
	public:
		virtual ~IDelegate() = default;
		virtual bool isType(const std::type_info& _type) = 0;
		virtual void invoke(Args... args) = 0;
		virtual bool compare(IDelegate* _delegate) const = 0;
		virtual bool compare(IDelegateUnlink* _unlink) const
		{
			return false;
		}
	};

	template <typename ...Args>
	class CStaticDelegate : public IDelegate<Args...>
	{
	public:
		using Func = void(*)(Args... args);

		CStaticDelegate(Func _func) : mFunc(_func) { }

		bool isType(const std::type_info& _type) override
		{
			return typeid(CStaticDelegate) == _type;
		}

		void invoke(Args... args) override
		{
			mFunc(args...);
		}

		bool compare(IDelegate<Args...>* _delegate) const override
		{
			if (nullptr == _delegate || !_delegate->isType(typeid(CStaticDelegate))) return false;
			auto cast = static_cast<CStaticDelegate*>(_delegate);
			return cast->mFunc == mFunc;
		}
		bool compare(IDelegateUnlink* _unlink) const override
		{
			return false;
		}

	private:
		Func mFunc;
	};

	template <typename T, typename ...Args>
	class CMethodDelegate : public IDelegate<Args...>
	{
	public:
		using Method = void (T::*)(Args... args);

		CMethodDelegate(IDelegateUnlink* _unlink, T* _object, Method _method) : mUnlink(_unlink), mObject(_object), mMethod(_method) { }

		bool isType(const std::type_info& _type) override
		{
			return typeid(CMethodDelegate) == _type;
		}

		void invoke(Args... args) override
		{
			(mObject->*mMethod)(args...);
		}

		bool compare(IDelegate<Args...>* _delegate) const override
		{
			if (nullptr == _delegate || !_delegate->isType(typeid(CMethodDelegate))) return false;
			auto cast = static_cast<CMethodDelegate*>(_delegate);
			return cast->mObject == mObject && cast->mMethod == mMethod;
		}

		bool compare(IDelegateUnlink* _unlink) const override
		{
			return mUnlink == _unlink;
		}

	private:
		IDelegateUnlink* mUnlink;
		T* mObject;
		Method mMethod;
	};

} // namespace delegates

// Creates delegate from a function or a static class method
template <typename ...Args>
inline delegates::IDelegate<Args...>* newDelegate(void(*_func)(Args... args))
{
	return new delegates::CStaticDelegate<Args...>(_func);
}

// Creates delegate from a non-static class method
template <typename T, typename ...Args>
inline delegates::IDelegate<Args...>* newDelegate(T* _object, void (T::*_method)(Args... args))
{
	return new delegates::CMethodDelegate<T, Args...>(delegates::GetDelegateUnlink(_object), _object, _method);
}

namespace delegates
{

	template <typename ...Args>
	class CDelegate
	{
	public:
		using IDelegate = IDelegate<Args...>;

		CDelegate() : mDelegate(nullptr) { }
		CDelegate(const CDelegate& _event) : mDelegate(nullptr)
		{
			// take ownership
			mDelegate = _event.mDelegate;
			const_cast<CDelegate&>(_event).mDelegate = nullptr;
		}

		~CDelegate()
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

		CDelegate<Args...>& operator=(IDelegate* _delegate)
		{
			delete mDelegate;
			mDelegate = _delegate;
			return *this;
		}

		CDelegate<Args...>& operator=(const CDelegate<Args...>& _event)
		{
			if (this == &_event)
				return *this;

			// take ownership
			IDelegate* del = _event.mDelegate;
			const_cast<CDelegate&>(_event).mDelegate = nullptr;

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
	class CMultiDelegate
	{
	public:
		using IDelegate = IDelegate<Args...>;
		using ListDelegate = typename std::list<IDelegate*>;

		CMultiDelegate() { }
		~CMultiDelegate()
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

		CMultiDelegate& operator+=(IDelegate* _delegate)
		{
			for (auto iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					MYGUI_EXCEPT("Trying to add same delegate twice.");
				}
			}
			mListDelegates.push_back(_delegate);
			return *this;
		}

		CMultiDelegate& operator-=(IDelegate* _delegate)
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
			return *this;
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

		CMultiDelegate(const CMultiDelegate& _event)
		{
			// take ownership
			ListDelegate del = _event.mListDelegates;
			const_cast<CMultiDelegate&>(_event).mListDelegates.clear();

			safe_clear(del);

			mListDelegates = del;
		}

		CMultiDelegate& operator=(const CMultiDelegate& _event)
		{
			// take ownership
			ListDelegate del = _event.mListDelegates;
			const_cast<CMultiDelegate&>(_event).mListDelegates.clear();

			safe_clear(del);

			mListDelegates = del;

			return *this;
		}

		MYGUI_OBSOLETE("use : operator += ")
		CMultiDelegate& operator=(IDelegate* _delegate)
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
	using CDelegate0 = CDelegate<>;
	template <typename ...Args>
	using CDelegate1 = CDelegate<Args...>;
	template <typename ...Args>
	using CDelegate2 = CDelegate<Args...>;
	template <typename ...Args>
	using CDelegate3 = CDelegate<Args...>;
	template <typename ...Args>
	using CDelegate4 = CDelegate<Args...>;
	template <typename ...Args>
	using CDelegate5 = CDelegate<Args...>;
	template <typename ...Args>
	using CDelegate6 = CDelegate<Args...>;

	using CMultiDelegate0 = CMultiDelegate<>;
	template <typename ...Args>
	using CMultiDelegate1 = CMultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate2 = CMultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate3 = CMultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate4 = CMultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate5 = CMultiDelegate<Args...>;
	template <typename ...Args>
	using CMultiDelegate6 = CMultiDelegate<Args...>;
//#endif
}

} // namespace MyGUI

#endif // MYGUI_DELEGATE_H_
