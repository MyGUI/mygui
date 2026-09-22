#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_Delegate.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

namespace
{

	using unittest::require;
	using Event = MyGUI::delegates::MultiDelegate<>;

	void increment(int& _value)
	{
		++_value;
	}

	struct Receiver : MyGUI::delegates::IDelegateUnlink
	{
		void add(int& _value)
		{
			_value += 10;
		}

		void addConst(int& _value) const
		{
			_value += 100;
		}
	};

	void testSingleDelegate()
	{
		MyGUI::delegates::Delegate<int&> delegate;
		int value = 0;
		require(delegate.empty(), "A new delegate must be empty");
		delegate(value);
		require(value == 0, "Invoking an empty delegate must do nothing");
		delegate = MyGUI::newDelegate(increment);
		delegate(value);
		require(value == 1 && !delegate.empty(), "A function delegate must receive reference arguments");

		Receiver receiver;
		delegate = MyGUI::newDelegate(&receiver, &Receiver::add);
		delegate(value);
		require(value == 11, "Assignment must replace the previous callback");
		const Receiver constReceiver;
		delegate = MyGUI::newDelegate(&constReceiver, &Receiver::addConst);
		delegate(value);
		require(value == 111, "Const member callbacks must receive reference arguments");
		delegate.clear();
		delegate(value);
		require(delegate.empty() && value == 111, "Clearing must remove the callback");
		delegate = MyGUI::newDelegate(increment);
		delegate = nullptr;
		require(delegate.empty(), "Assigning nullptr must clear the callback");
	}

	void testSingleOwnership()
	{
		std::weak_ptr<int> lifetime;
		{
			MyGUI::delegates::Delegate<> delegate;
			auto owner = std::make_shared<int>(0);
			lifetime = owner;
			delegate = MyGUI::newDelegate([owner]() { ++*owner; }, 1);
			owner.reset();
			require(!lifetime.expired(), "A delegate must retain its callback captures");
			delegate = MyGUI::newDelegate([]() {}, 2);
			require(lifetime.expired(), "Replacing a callback must release its captures");

			owner = std::make_shared<int>(0);
			lifetime = owner;
			delegate = MyGUI::newDelegate([owner]() { ++*owner; }, 3);
			owner.reset();
		}
		require(lifetime.expired(), "Destroying a delegate must release its captures");
	}

	void testCallbackIdentity()
	{
		MyGUI::delegates::MultiDelegate<int&> event;
		Receiver first;
		Receiver second;
		event += MyGUI::newDelegate(increment);
		event += MyGUI::newDelegate(&first, &Receiver::add);
		event += MyGUI::newDelegate(&second, &Receiver::add);
		event += MyGUI::newDelegate(&first, &Receiver::addConst);
		int value = 0;
		event(value);
		require(value == 121, "Callbacks for distinct objects and methods must coexist");
		event -= MyGUI::newDelegate(&first, &Receiver::add);
		event -= MyGUI::newDelegate(increment);
		value = 0;
		event(value);
		require(value == 110, "Unsubscription must match both object and method, or the free function");
		event -= MyGUI::newDelegate(&first, &Receiver::addConst);
		value = 0;
		event(value);
		require(value == 10, "Const member callbacks must be removable by identity");
	}

	void testInheritedMemberCallbacks()
	{
		struct DerivedReceiver : Receiver
		{
		};

		DerivedReceiver receiver;
		const DerivedReceiver constReceiver;
		MyGUI::delegates::MultiDelegate<int&> event;
		event += MyGUI::newDelegate(&receiver, &Receiver::add);
		event += MyGUI::newDelegate(&receiver, &Receiver::addConst);
		event += MyGUI::newDelegate(&constReceiver, &Receiver::addConst);
		int value = 0;
		event(value);
		require(value == 210, "Inherited member callbacks must support mutable and const derived objects");

		event -= MyGUI::newDelegate(&receiver, &Receiver::add);
		value = 0;
		event(value);
		require(value == 200, "Inherited non-const member callbacks must be removable by identity");

		event.clear(&receiver);
		value = 0;
		event(value);
		require(value == 100, "Unlinking a derived receiver must preserve callbacks for other receivers");

		event -= MyGUI::newDelegate(&constReceiver, &Receiver::addConst);
		require(event.empty(), "Inherited const member callbacks must be removable by identity");
	}

	void testExplicitMemberTemplateArguments()
	{
		Receiver receiver;
		const Receiver constReceiver;
		MyGUI::delegates::Delegate<int&> delegate;
		delegate = MyGUI::newDelegate<Receiver, int&>(&receiver, &Receiver::add);
		int value = 0;
		delegate(value);
		delegate = MyGUI::newDelegate<Receiver, int&>(&constReceiver, &Receiver::addConst);
		delegate(value);
		require(value == 110, "Explicit member template arguments must retain their original order");
	}

	void testFunctionIdsAndOrder()
	{
		Event event;
		std::vector<int> calls;
		std::function<void()> callback = [&calls]()
		{
			calls.push_back(1);
		};
		event += MyGUI::newDelegate(callback, 1);
		event += MyGUI::newDelegate([&calls]() { calls.push_back(2); }, 2);
		event();
		require(calls == std::vector<int>({1, 2}), "Callbacks must run in subscription order");
		event -= MyGUI::newDelegate([]() {}, 1);
		calls.clear();
		event();
		require(calls == std::vector<int>{2}, "A function callback must be removable using its ID");
		event -= MyGUI::newDelegate(callback, 2);
		require(event.empty(), "A lambda callback must be removable using its ID");
	}

	void testDuplicateSubscription()
	{
		// Duplicate rejection logs through the engine's LogManager.
		unittest::TestContext context;
		Event event;
		int calls = 0;
		auto callback = [&calls]()
		{
			++calls;
		};
		event += MyGUI::newDelegate(callback, 1);
		// Ownership transfers only on successful subscription.
		std::unique_ptr<Event::IDelegate> duplicate(MyGUI::newDelegate(callback, 1));
		bool threw = false;
		try
		{
			event += duplicate.get();
			duplicate.release();
		}
		catch (const MyGUI::Exception&)
		{
			threw = true;
		}
		require(threw, "Duplicate callback IDs must be rejected");
		event();
		require(calls == 1, "Rejected subscriptions must leave the original callback intact");
	}

	void testEmptyAndRemoval()
	{
		Event event;
		event();
		event += nullptr;
		event -= nullptr;
		event -= MyGUI::newDelegate([]() {}, 99);
		event.clear(nullptr);
		require(event.empty(), "Null and absent subscriptions must leave an empty event unchanged");
		int calls = 0;
		auto* callback = MyGUI::newDelegate([&calls]() { ++calls; }, 1);
		event += callback;
		event -= MyGUI::newDelegate([]() {}, 99);
		event();
		require(calls == 1, "Removing an absent listener must preserve existing listeners");
		event -= callback;
		event();
		require(event.empty() && calls == 1, "Removing the original delegate pointer must be safe");
	}

	void testUnlinkReceiver()
	{
		MyGUI::delegates::MultiDelegate<int&> event;
		Receiver first;
		Receiver second;
		event += MyGUI::newDelegate(&first, &Receiver::add);
		event += MyGUI::newDelegate(&first, &Receiver::addConst);
		event += MyGUI::newDelegate(&second, &Receiver::add);
		event += MyGUI::newDelegate(increment);
		event.clear(&first);
		event.clear(&first);
		int value = 0;
		event(value);
		require(value == 11, "Unlinking must remove all callbacks for only the specified receiver");
	}

	void testMultiOwnership()
	{
		std::weak_ptr<int> lifetime;
		{
			Event event;
			auto owner = std::make_shared<int>(0);
			lifetime = owner;
			event += MyGUI::newDelegate([owner]() { ++*owner; }, 1);
			owner.reset();
			event();
			require(!lifetime.expired() && *lifetime.lock() == 1, "An event must retain and invoke its captures");
			event -= MyGUI::newDelegate([]() {}, 1);
			require(lifetime.expired(), "Removing a listener must release its captures");

			owner = std::make_shared<int>(0);
			lifetime = owner;
			event += MyGUI::newDelegate([owner]() { ++*owner; }, 2);
			owner.reset();
			event.clear();
			require(lifetime.expired(), "Clearing an event must release its captures");

			owner = std::make_shared<int>(0);
			lifetime = owner;
			event += MyGUI::newDelegate([owner]() { ++*owner; }, 3);
			owner.reset();
		}
		require(lifetime.expired(), "Destroying an event must release its captures");
	}

	struct MutatingReceiver
	{
		Event event;
		std::vector<int> calls;
		bool nested = false;

		void first()
		{
			calls.push_back(1);
		}

		void second()
		{
			calls.push_back(2);
		}

		void removeSelf()
		{
			calls.push_back(1);
			event -= MyGUI::newDelegate(this, &MutatingReceiver::removeSelf);
		}

		void addNext()
		{
			calls.push_back(1);
			event -= MyGUI::newDelegate(this, &MutatingReceiver::addNext);
			event += MyGUI::newDelegate(this, &MutatingReceiver::second);
		}

		void removeNext()
		{
			calls.push_back(1);
			event -= MyGUI::newDelegate(this, &MutatingReceiver::second);
		}

		void clearAll()
		{
			calls.push_back(1);
			event.clear();
		}

		void recurse()
		{
			calls.push_back(nested ? 3 : 1);
			if (!nested)
			{
				nested = true;
				event();
				nested = false;
			}
		}

		void recurseAndClear()
		{
			calls.push_back(nested ? 3 : 1);
			if (!nested)
			{
				nested = true;
				event();
				// A nested return must leave the outer dispatch protected against list erasure.
				event.clear();
			}
		}
	};

	void testSelfRemoval()
	{
		MutatingReceiver receiver;
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::removeSelf);
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::second);
		receiver.event();
		receiver.event();
		require(receiver.calls == std::vector<int>({1, 2, 2}), "Self-removal must preserve remaining listeners");
	}

	void testRemovePendingListener()
	{
		MutatingReceiver receiver;
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::removeNext);
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::second);
		receiver.event();
		receiver.event();
		require(receiver.calls == std::vector<int>({1, 1}), "A removed pending listener must not run");
	}

	void testAddDuringDispatch()
	{
		MutatingReceiver receiver;
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::addNext);
		receiver.event();
		require(
			receiver.calls == std::vector<int>({1, 2}),
			"Appended listeners must participate in the current dispatch");
		receiver.event();
		require(receiver.calls == std::vector<int>({1, 2, 2}), "The appended listener must remain subscribed");
	}

	void testClearDuringDispatch()
	{
		MutatingReceiver receiver;
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::clearAll);
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::second);
		receiver.event();
		require(receiver.event.empty(), "Clearing during dispatch must remove all listeners");
		receiver.event();
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::second);
		receiver.event();
		require(receiver.calls == std::vector<int>({1, 2}), "Clearing must skip pending callbacks and permit reuse");
	}

	void testNestedDispatch()
	{
		MutatingReceiver receiver;
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::recurse);
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::second);
		receiver.event();
		require(receiver.calls == std::vector<int>({1, 3, 2, 2}), "Nested dispatch must resume the outer invocation");
	}

	void testClearAfterNestedDispatch()
	{
		MutatingReceiver receiver;
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::recurseAndClear);
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::second);
		receiver.event();
		require(receiver.event.empty(), "Clearing after nested dispatch must remove all listeners");
		require(receiver.calls == std::vector<int>({1, 3, 2}), "The cleared outer listener must not run again");
		receiver.event += MyGUI::newDelegate(&receiver, &MutatingReceiver::first);
		receiver.event();
		require(receiver.calls.back() == 1, "The event must remain reusable after nested mutation");
	}

	void testExceptionRecovery()
	{
		Event event;
		std::vector<int> calls;
		event += MyGUI::newDelegate(
			[&calls]()
			{
				calls.push_back(1);
				throw std::runtime_error("callback failure");
			},
			1);
		event += MyGUI::newDelegate([&calls]() { calls.push_back(2); }, 2);
		bool threw = false;
		try
		{
			event();
		}
		catch (const std::runtime_error& error)
		{
			threw = std::string_view(error.what()) == "callback failure";
		}
		require(threw && calls == std::vector<int>{1}, "Callback exceptions must propagate and stop dispatch");
		event -= MyGUI::newDelegate([]() {}, 1);
		event();
		require(calls == std::vector<int>({1, 2}), "Dispatch must recover after removing a throwing callback");
		event.clear();
		event += MyGUI::newDelegate([&calls]() { calls.push_back(3); }, 3);
		event();
		require(calls == std::vector<int>({1, 2, 3}), "An event must support clearing and reuse after an exception");
	}

}

int main()
{
	return unittest::runTests({
		{"Single delegate invocation and replacement", testSingleDelegate},
		{"Single delegate ownership", testSingleOwnership},
		{"Callback identity", testCallbackIdentity},
		{"Inherited member callbacks", testInheritedMemberCallbacks},
		{"Explicit member template arguments backward compatibility", testExplicitMemberTemplateArguments},
		{"Function IDs and invocation order", testFunctionIdsAndOrder},
		{"Duplicate subscription", testDuplicateSubscription},
		{"Empty events and removal", testEmptyAndRemoval},
		{"Receiver unlinking", testUnlinkReceiver},
		{"Multi delegate ownership", testMultiOwnership},
		{"Self-removal", testSelfRemoval},
		{"Remove a pending listener", testRemovePendingListener},
		{"Add during dispatch", testAddDuringDispatch},
		{"Clear during dispatch", testClearDuringDispatch},
		{"Nested dispatch", testNestedDispatch},
		{"Clear after nested dispatch", testClearAfterNestedDispatch},
		{"Exception recovery", testExceptionRecovery},
	});
}
