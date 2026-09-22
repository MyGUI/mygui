/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef SIGSLOT_H_
#define SIGSLOT_H_

#include <algorithm>
#include <any>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace sigslot
{

	// Editor signals are synchronous and single-threaded. All operations, including
	// receiver destruction, must take place on the same thread.
	struct single_threaded
	{
	};

	template<typename... Args>
	class signal;

	namespace detail
	{

		struct connection_state
		{
			virtual ~connection_state() = default;
			bool connected = true;
		};

	}

	// Receivers disconnect automatically at base destruction. Call disconnect_all()
	// earlier if a derived destructor emits signals while tearing down its members.
	// Keep this base non-template: MSVC implicitly exports template bases of exported
	// editor classes, conflicting with instantiations in the editor executables.
	class has_slots_base
	{
	public:
		has_slots_base() = default;
		has_slots_base(const has_slots_base&) = delete;
		has_slots_base& operator=(const has_slots_base&) = delete;
		has_slots_base(has_slots_base&&) = delete;
		has_slots_base& operator=(has_slots_base&&) = delete;

		virtual ~has_slots_base()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			for (const auto& weak : mConnections)
			{
				if (auto connection = weak.lock())
					connection->connected = false;
			}
			mConnections.clear();
		}

	private:
		template<typename... Args>
		friend class signal;

		void track(const std::shared_ptr<detail::connection_state>& _connection) const
		{
			mConnections.erase(
				std::remove_if(
					mConnections.begin(),
					mConnections.end(),
					[](const auto& weak)
					{
						auto connection = weak.lock();
						return !connection || !connection->connected;
					}),
				mConnections.end());
			mConnections.push_back(_connection);
		}

		mutable std::vector<std::weak_ptr<detail::connection_state>> mConnections;
	};

	template<typename Policy = single_threaded>
	using has_slots = std::enable_if_t<std::is_same_v<Policy, single_threaded>, has_slots_base>;

	// Slots run in connection order. Each emission takes a snapshot: new slots are
	// visible to subsequent (including nested) emissions, and disconnected slots
	// are skipped immediately. Signals and receivers have stable, noncopyable identities.
	template<typename... Args>
	class signal
	{
		static_assert((!std::is_rvalue_reference_v<Args> && ...), "Multicast signals cannot consume rvalue arguments");

		struct connection : detail::connection_state
		{
			std::function<void(Args...)> callback;
			const has_slots<>* receiver = nullptr;
			std::any method;
		};

	public:
		signal() = default;
		signal(const signal&) = delete;
		signal& operator=(const signal&) = delete;
		signal(signal&&) = delete;
		signal& operator=(signal&&) = delete;

		~signal()
		{
			disconnect_all();
		}

		template<typename Object, typename Method>
		void connect(Object* _object, Method _method)
		{
			static_assert(std::is_member_function_pointer_v<Method>, "Expected a member function");
			auto slot = std::make_shared<connection>();
			slot->receiver = _object;
			slot->method = _method;
			slot->callback = [_object, _method](Args... args)
			{
				std::invoke(_method, _object, args...);
			};
			// Track first so that allocation failure cannot leave an untracked live slot.
			slot->receiver->track(slot);
			removeDisconnected();
			mConnections.push_back(std::move(slot));
		}

		// Keep an existing connection in place; return true only when adding a new one.
		template<typename Object, typename Method>
		bool connect_unique(Object* _object, Method _method)
		{
			if (exist(_object, _method))
				return false;
			connect(_object, _method);
			return true;
		}

		// Free functions and lambdas live until disconnect_all() or signal destruction.
		// Captured objects are not tracked; use the member overload for automatic disconnection.
		template<typename Callable>
		void connect(Callable&& _callback)
		{
			auto slot = std::make_shared<connection>();
			slot->callback = std::forward<Callable>(_callback);
			removeDisconnected();
			mConnections.push_back(std::move(slot));
		}

		template<typename Object, typename Method>
		bool exist(Object* _object, Method _method) const
		{
			const has_slots<>* receiver = _object;
			return std::any_of(
				mConnections.begin(),
				mConnections.end(),
				[receiver, _method](const auto& slot)
				{
					const auto* method = std::any_cast<Method>(&slot->method);
					return slot->connected && slot->receiver == receiver && method && *method == _method;
				});
		}

		// Remove every connection to the receiver, including duplicate subscriptions.
		void disconnect(const has_slots<>* _receiver)
		{
			for (const auto& slot : mConnections)
			{
				if (slot->receiver == _receiver)
					slot->connected = false;
			}
			removeDisconnected();
		}

		void disconnect_all()
		{
			for (const auto& slot : mConnections)
				slot->connected = false;
			mConnections.clear();
		}

		void emit(Args... args)
		{
			removeDisconnected();
			const auto connections = mConnections;
			// Do not access this after invoking a callback: it may destroy the signal.
			for (const auto& slot : connections)
			{
				if (slot->connected)
					slot->callback(args...);
			}
		}

		void operator()(Args... args)
		{
			emit(args...);
		}

	private:
		void removeDisconnected()
		{
			mConnections.erase(
				std::remove_if(
					mConnections.begin(),
					mConnections.end(),
					[](const auto& slot) { return !slot->connected; }),
				mConnections.end());
		}

		std::vector<std::shared_ptr<connection>> mConnections;
	};

	namespace detail
	{

		template<typename Policy, typename... Args>
		using signal_with_policy = std::enable_if_t<std::is_same_v<Policy, single_threaded>, signal<Args...>>;

	}

	// Legacy arity names remain available; signal<Args...> has no arity limit.
	template<typename Policy = single_threaded>
	using signal0 = detail::signal_with_policy<Policy>;
	template<typename A1, typename Policy = single_threaded>
	using signal1 = detail::signal_with_policy<Policy, A1>;
	template<typename A1, typename A2, typename Policy = single_threaded>
	using signal2 = detail::signal_with_policy<Policy, A1, A2>;
	template<typename A1, typename A2, typename A3, typename Policy = single_threaded>
	using signal3 = detail::signal_with_policy<Policy, A1, A2, A3>;
	template<typename A1, typename A2, typename A3, typename A4, typename Policy = single_threaded>
	using signal4 = detail::signal_with_policy<Policy, A1, A2, A3, A4>;
	template<typename A1, typename A2, typename A3, typename A4, typename A5, typename Policy = single_threaded>
	using signal5 = detail::signal_with_policy<Policy, A1, A2, A3, A4, A5>;
	template<
		typename A1,
		typename A2,
		typename A3,
		typename A4,
		typename A5,
		typename A6,
		typename Policy = single_threaded>
	using signal6 = detail::signal_with_policy<Policy, A1, A2, A3, A4, A5, A6>;
	template<
		typename A1,
		typename A2,
		typename A3,
		typename A4,
		typename A5,
		typename A6,
		typename A7,
		typename Policy = single_threaded>
	using signal7 = detail::signal_with_policy<Policy, A1, A2, A3, A4, A5, A6, A7>;
	template<
		typename A1,
		typename A2,
		typename A3,
		typename A4,
		typename A5,
		typename A6,
		typename A7,
		typename A8,
		typename Policy = single_threaded>
	using signal8 = detail::signal_with_policy<Policy, A1, A2, A3, A4, A5, A6, A7, A8>;

}

#endif
