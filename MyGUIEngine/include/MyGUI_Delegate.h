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
#include <utility>
#include <vector>

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

		namespace detail
		{

			struct DelegateFactory;

		}

		template<typename... Args>
		class Delegate;
		template<typename... Args>
		class MultiDelegate;

		// Callables have stable addresses. Ownership and dispatch retention are single-threaded.
		template<typename... Args>
		class DelegateFunction
		{
		public:
			using Function = std::function<void(Args...)>;

			DelegateFunction(const DelegateFunction&) = delete;
			DelegateFunction& operator=(const DelegateFunction&) = delete;
			virtual ~DelegateFunction() = default;

			void invoke(Args... args)
			{
				mInvoke(this, std::forward<Args>(args)...);
			}

			bool compare(DelegateFunction* _delegate) const
			{
				return _delegate && _delegate->mObject == mObject &&
					_delegate->mFunctionPointer.compare(mFunctionPointer);
			}

			bool compare(IDelegateUnlink* _unlink) const
			{
				return mUnlink == _unlink;
			}

		private:
			friend class Delegate<Args...>;
			friend class MultiDelegate<Args...>;
			friend struct detail::DelegateFactory;

			DelegateFunction(
				void (*_invoke)(DelegateFunction*, Args&&...),
				Any _identity,
				const void* _object,
				const IDelegateUnlink* _unlink) :
				mInvoke(_invoke),
				mUnlink(_unlink),
				mObject(_object),
				mFunctionPointer(std::move(_identity))
			{
			}

			void release() noexcept
			{
				if (--mReferences == 0)
					delete this;
			}

			void (*mInvoke)(DelegateFunction*, Args&&...);
			size_t mReferences{1};
			DelegateFunction* mNextRetired{nullptr};
			const void* mOwner{nullptr};
			const IDelegateUnlink* mUnlink{nullptr};
			const void* mObject{nullptr};
			Any mFunctionPointer;
		};

		namespace detail
		{

			struct DelegateFactory
			{
				template<typename Result, typename Callable>
				struct Storage;

				template<typename Callable, typename... Args>
				struct Storage<DelegateFunction<Args...>, Callable> final : DelegateFunction<Args...>
				{
					using Result = DelegateFunction<Args...>;
					Storage(Callable _function, Any _identity, const void* _object, const IDelegateUnlink* _unlink) :
						Result(&invoke, std::move(_identity), _object, _unlink),
						function(std::move(_function))
					{
					}

					static void invoke(Result* _self, Args&&... args)
					{
						std::invoke(static_cast<Storage*>(_self)->function, std::forward<Args>(args)...);
					}

					Callable function;
				};

				template<typename Result, typename Callable>
				static Result* create(
					Callable _function,
					Any _identity,
					const void* _object = nullptr,
					const IDelegateUnlink* _unlink = nullptr)
				{
					return new Storage<Result, Callable>(std::move(_function), std::move(_identity), _object, _unlink);
				}

				template<typename Result, typename Callable>
				static Result* create(Callable _function, Any _identity, const IDelegateUnlink* _object)
				{
					return create<Result>(std::move(_function), std::move(_identity), _object, _object);
				}
			};

		}

	} // namespace delegates

	// Creates delegate from a function or a static class method
	template<typename... Args>
	inline delegates::DelegateFunction<Args...>* newDelegate(void (*_func)(Args... args))
	{
		if (!_func)
			return delegates::detail::DelegateFactory::create<delegates::DelegateFunction<Args...>>(
				std::function<void(Args...)>{},
				_func);
		return delegates::detail::DelegateFactory::create<delegates::DelegateFunction<Args...>>(_func, _func);
	}

	// Creates delegate from a non-static class method
	template<typename T, typename... Args, typename Owner>
	inline delegates::DelegateFunction<Args...>* newDelegate(T* _object, void (Owner::*_method)(Args... args))
	{
		return delegates::detail::DelegateFactory::create<delegates::DelegateFunction<Args...>>(
			[_object, _method](Args&&... args) { std::invoke(_method, _object, std::forward<Args>(args)...); },
			_method,
			_object);
	}
	template<typename T, typename... Args, typename Owner>
	inline delegates::DelegateFunction<Args...>* newDelegate(
		const T* _object,
		void (Owner::*_method)(Args... args) const)
	{
		return delegates::detail::DelegateFactory::create<delegates::DelegateFunction<Args...>>(
			[_object, _method](Args&&... args) { std::invoke(_method, _object, std::forward<Args>(args)...); },
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
		return delegates::detail::DelegateFactory::create<delegates::DelegateFunction<Args...>>(_function, delegateId);
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
		return delegates::detail::DelegateFactory::create<DelegateType>(_function, delegateId);
	}

	namespace delegates
	{

		// Operations on an event and its receivers must be performed on one thread.
		template<typename... Args>
		class Delegate
		{
		public:
			using IDelegate = DelegateFunction<Args...>;

			Delegate() = default;
			Delegate(Delegate&& _other) noexcept :
				mDelegate(std::exchange(_other.mDelegate, nullptr))
			{
			}

			Delegate& operator=(Delegate&& _other) noexcept
			{
				if (this != &_other)
					*this = std::exchange(_other.mDelegate, nullptr);
				return *this;
			}

			~Delegate()
			{
				mDestroying = true;
				clear();
			}

			bool empty() const
			{
				return mDelegate == nullptr;
			}

			void clear()
			{
				if (auto* old = std::exchange(mDelegate, nullptr))
					old->release();
			}

			Delegate& operator=(IDelegate* _delegate)
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

			void operator()(Args... args) const
			{
				if (auto* delegate = mDelegate)
				{
					Invocation guard(delegate);
					delegate->invoke(std::forward<Args>(args)...);
				}
			}

		private:
			class Invocation
			{
			public:
				explicit Invocation(IDelegate* _delegate) :
					mDelegate(_delegate)
				{
					++mDelegate->mReferences;
				}
				Invocation(const Invocation&) = delete;
				Invocation& operator=(const Invocation&) = delete;
				~Invocation()
				{
					mDelegate->release();
				}

			private:
				IDelegate* mDelegate;
			};

			IDelegate* mDelegate{nullptr};
			bool mDestroying{false};
		};

		template<typename... Args>
		class MultiDelegate
		{
		public:
			using IDelegate = DelegateFunction<Args...>;
			using ListDelegate = std::list<std::unique_ptr<IDelegate>>;

			MultiDelegate() = default;
			MultiDelegate(MultiDelegate&& _other) noexcept :
				mState(std::exchange(_other.mState, nullptr))
			{
			}

			~MultiDelegate()
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

			bool empty() const
			{
				return !mState || mState->active == 0;
			}

			void clear()
			{
				if (auto* state = mState)
				{
					Operation guard(state);
					state->clear();
				}
			}

			void clear(IDelegateUnlink* _unlink)
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

			void operator+=(IDelegate* _delegate)
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

			void operator-=(IDelegate* _delegate)
			{
				if (!_delegate)
					return;
				if (auto* state = mState)
				{
					Operation guard(state);
					// The original pointer can also be supplied; never delete an owned or retired callback here.
					std::unique_ptr<IDelegate> token(_delegate->mOwner ? nullptr : _delegate);
					const auto index = state->find(_delegate);
					if (index != state->callbacks.size())
						state->remove(index);
				}
				else if (!_delegate->mOwner)
					delete _delegate;
			}

			void operator()(Args... args) const
			{
				auto* state = mState;
				if (!state || state->active == 0)
					return;
				Operation guard(state);
				// Callbacks may reallocate the vector or destroy the event. Reload by index using retained state.
				size_t i = 0;
				do
				{
					if (auto* callback = state->callbacks[i])
						callback->invoke(args...);
				} while (++i < state->extent);
			}

			MYGUI_OBSOLETE("use : operator += ")
			MultiDelegate& operator=(IDelegate* _delegate)
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

		private:
			struct State
			{
				std::vector<IDelegate*> callbacks;
				// Avoid subtracting vector pointers after every callback in the dispatch loop.
				size_t extent{0};
				size_t active{0};
				size_t references{1}; // Event ownership plus active operations, including nested invocations.
				IDelegate* retired{nullptr};
				IDelegate* retiredTail{nullptr};
				bool closed{false};

				size_t find(IDelegate* _delegate) const
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

				void add(IDelegate* _delegate)
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

				void remove(size_t _index)
				{
					auto* callback = std::exchange(callbacks[_index], nullptr);
					--active;
					if (retiredTail)
						retiredTail->mNextRetired = callback;
					else
						retired = callback;
					retiredTail = callback;
				}

				void clear()
				{
					for (size_t i = 0; i < callbacks.size(); ++i)
					{
						if (callbacks[i])
							remove(i);
					}
				}

				void collectIfIdle()
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
			};

			class Operation
			{
			public:
				explicit Operation(State* _state) :
					mState(_state)
				{
					++mState->references;
				}
				Operation(const Operation&) = delete;
				Operation& operator=(const Operation&) = delete;
				~Operation()
				{
					if (mState->retired)
						mState->collectIfIdle();
					if (--mState->references == 0)
						delete mState;
				}

			private:
				State* mState;
			};

			State* mState{nullptr};
			bool mDestroying{false};
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
