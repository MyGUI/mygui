/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DELEGATE_H_
#define MYGUI_DELEGATE_H_

#include "MyGUI_Diagnostic.h"
#include "MyGUI_Any.h"
#include <algorithm>
#include <list>
#include <memory>
#include <functional>

namespace MyGUI
{

	namespace delegates
	{
		// base class for unsubscribing from multi delegates
		class MYGUI_EXPORT IDelegateUnlink
		{
		public:
			virtual ~IDelegateUnlink() = default;

			IDelegateUnlink() :
				m_baseDelegateUnlink(this)
			{
			}
			bool compare(IDelegateUnlink* _unlink) const
			{
				return m_baseDelegateUnlink == _unlink->m_baseDelegateUnlink;
			}

		private:
			IDelegateUnlink* m_baseDelegateUnlink;
		};

		template<typename... Args>
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
			DelegateFunction(Function _function, Any _functionPointer, const IDelegateUnlink* _object) :
				mFunction(_function),
				mUnlink(_object),
				mObject(_object),
				mFunctionPointer(_functionPointer)
			{
			}

			// non-static class method
			DelegateFunction(Function _function, Any _functionPointer, const void* _object) :
				mFunction(_function),
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
				if (nullptr == _delegate)
					return false;
				return _delegate->mObject == mObject && _delegate->mFunctionPointer.compare(mFunctionPointer);
			}

			bool compare(IDelegateUnlink* _unlink) const
			{
				return mUnlink == _unlink;
			}

		private:
			Function mFunction;

			const IDelegateUnlink* mUnlink = nullptr;
			const void* mObject = nullptr;
			Any mFunctionPointer;
		};

	} // namespace delegates

	// Creates delegate from a function or a static class method
	template<typename... Args>
	inline delegates::DelegateFunction<Args...>* newDelegate(void (*_func)(Args... args))
	{
		return new delegates::DelegateFunction<Args...>(_func, _func);
	}

	// Creates delegate from a non-static class method
	template<typename T, typename... Args>
	inline delegates::DelegateFunction<Args...>* newDelegate(T* _object, void (T::*_method)(Args... args))
	{
		return new delegates::DelegateFunction<Args...>(
			[=](Args&&... args) { return (_object->*_method)(std::forward<decltype(args)>(args)...); },
			_method,
			_object);
	}
	template<typename T, typename... Args>
	inline delegates::DelegateFunction<Args...>* newDelegate(const T* _object, void (T::*_method)(Args... args) const)
	{
		return new delegates::DelegateFunction<Args...>(
			[=](Args&&... args) { return (_object->*_method)(std::forward<decltype(args)>(args)...); },
			_method,
			_object);
	}

	// Creates delegate from std::function
	// Require some user-defined delegateId, that should be used if operator-= is called to remove delegate.
	// delegateId need to be unique within single delegate.
	template<typename... Args>
	inline delegates::DelegateFunction<Args...>* newDelegate(
		const std::function<void(Args...)>& _function,
		int64_t delegateId)
	{
		return new delegates::DelegateFunction<Args...>(_function, delegateId);
	}


	template<typename>
	struct GetDelegateFunctionFromLambda;
	template<typename R, typename C, typename... Args>
	struct GetDelegateFunctionFromLambda<R (C::*)(Args...) const>
	{
		using type = MyGUI::delegates::DelegateFunction<Args...>;
	};

	// Creates delegate from lambda
	// Require some user-defined delegateId, that should be used if operator-= is called to remove delegate.
	// delegateId need to be unique within single delegate.
	template<typename TLambda>
	inline auto newDelegate(const TLambda& _function, int64_t delegateId)
	{
		using DelegateType = typename GetDelegateFunctionFromLambda<decltype(&TLambda::operator())>::type;
		return new DelegateType(_function, delegateId);
	}

	namespace delegates
	{

		template<typename... Args>
		class Delegate
		{
		public:
			using IDelegate = DelegateFunction<Args...>;

			bool empty() const
			{
				return !mDelegate;
			}

			void clear()
			{
				mDelegate.reset();
			}

			Delegate& operator=(IDelegate* _delegate)
			{
				mDelegate.reset(_delegate);
				return *this;
			}

			void operator()(Args... args) const
			{
				if (mDelegate)
					mDelegate->invoke(args...);
			}

		private:
			std::unique_ptr<IDelegate> mDelegate;
		};

		template<typename... Args>
		class MultiDelegate
		{
		public:
			using IDelegate = DelegateFunction<Args...>;
			using ListDelegate = typename std::list<std::unique_ptr<IDelegate>>;

			// These shouldn't be necessary, but MSVC (17.6.5) requires them anyway
			MultiDelegate() = default;
			MultiDelegate(MultiDelegate&&) noexcept = default;

			bool empty() const
			{
				for (const auto& delegate : mListDelegates)
				{
					if (delegate)
						return false;
				}
				return true;
			}

			void clear()
			{
				if (mRunning)
				{
					for (auto& delegate : mListDelegates)
						delegate.reset();
				}
				else
					mListDelegates.clear();
			}

			void clear(IDelegateUnlink* _unlink)
			{
				if (!_unlink)
					return;
				for (auto& delegate : mListDelegates)
				{
					if (delegate && delegate->compare(_unlink))
						delegate.reset();
				}
			}

			void operator+=(IDelegate* _delegate)
			{
				if (!_delegate)
					return;
				auto found = std::find_if(
					mListDelegates.begin(),
					mListDelegates.end(),
					[=](const auto& delegate) { return delegate && delegate->compare(_delegate); });
				if (found != mListDelegates.end())
					MYGUI_EXCEPT("Trying to add same delegate twice.");
				mListDelegates.emplace_back(_delegate);
			}

			void operator-=(IDelegate* _delegate)
			{
				if (!_delegate)
					return;
				auto found = std::find_if(
					mListDelegates.begin(),
					mListDelegates.end(),
					[=](const auto& delegate) { return delegate && delegate->compare(_delegate); });
				if (found != mListDelegates.end())
				{
					if (found->get() == _delegate)
						_delegate = nullptr;
					found->reset();
				}
				delete _delegate;
			}

			void operator()(Args... args) const
			{
				bool canErase = !mRunning;
				InvocationModificationGuard guard(*this);
				for (auto it = mListDelegates.begin(); it != mListDelegates.end();)
				{
					if (*it)
						(*it)->invoke(args...);
					else if (canErase)
					{
						it = mListDelegates.erase(it);
						continue;
					}
					++it;
				}
			}

			MYGUI_OBSOLETE("use : operator += ")
			MultiDelegate& operator=(IDelegate* _delegate)
			{
				clear();
				*this += _delegate;
				return *this;
			}

		private:
			mutable ListDelegate mListDelegates;
			mutable bool mRunning{false};

			class InvocationModificationGuard
			{
				const MultiDelegate* mDelegate;

			public:
				InvocationModificationGuard(const MultiDelegate& delegate)
				{
					if (delegate.mRunning)
						mDelegate = nullptr;
					else
					{
						mDelegate = &delegate;
						mDelegate->mRunning = true;
					}
				}
				~InvocationModificationGuard()
				{
					if (mDelegate)
						mDelegate->mRunning = false;
				}
			};
			friend class InvocationModificationGuard;
		};

#ifndef MYGUI_DONT_USE_OBSOLETE
		using CDelegate0 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate<>") = Delegate<>;
		template<typename... Args>
		using CDelegate1 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate") = Delegate<Args...>;
		template<typename... Args>
		using CDelegate2 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate") = Delegate<Args...>;
		template<typename... Args>
		using CDelegate3 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate") = Delegate<Args...>;
		template<typename... Args>
		using CDelegate4 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate") = Delegate<Args...>;
		template<typename... Args>
		using CDelegate5 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate") = Delegate<Args...>;
		template<typename... Args>
		using CDelegate6 MYGUI_OBSOLETE("use : MyGUI::delegates::Delegate") = Delegate<Args...>;

		using CMultiDelegate0 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate<>") = MultiDelegate<>;
		template<typename... Args>
		using CMultiDelegate1 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate") = MultiDelegate<Args...>;
		template<typename... Args>
		using CMultiDelegate2 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate") = MultiDelegate<Args...>;
		template<typename... Args>
		using CMultiDelegate3 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate") = MultiDelegate<Args...>;
		template<typename... Args>
		using CMultiDelegate4 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate") = MultiDelegate<Args...>;
		template<typename... Args>
		using CMultiDelegate5 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate") = MultiDelegate<Args...>;
		template<typename... Args>
		using CMultiDelegate6 MYGUI_OBSOLETE("use : MyGUI::delegates::MultiDelegate") = MultiDelegate<Args...>;
#endif
	}

} // namespace MyGUI

#endif // MYGUI_DELEGATE_H_
