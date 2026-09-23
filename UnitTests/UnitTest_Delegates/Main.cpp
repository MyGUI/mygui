#include "TestSupport.h"
#include "TestRunner.h"
#include <array>
#include "MyGUI_Delegate.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
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
		// Explicit template arguments must retain the object-type, argument-types order.
		delegate = MyGUI::newDelegate<Receiver, int&>(&receiver, &Receiver::add);
		delegate(value);
		require(value == 11, "Assignment must replace the previous callback");
		const Receiver constReceiver;
		delegate = MyGUI::newDelegate<Receiver, int&>(&constReceiver, &Receiver::addConst);
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

	// Clearing elements does not invalidate the captured reference to the vector itself.
	MYGUI_SUPPRESS_CLANG("-Wlifetime-safety-invalidation")
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
	MYGUI_UNSUPPRESS_CLANG()

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
		unittest::requireThrows<MyGUI::Exception>(
			[&]
			{
				event += duplicate.get();
				duplicate.release();
			},
			"Duplicate callback IDs must be rejected");
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

	void testSingleCallbackLifetime()
	{
		for (int mutation = 0; mutation < 3; ++mutation)
		{
			auto delegate = std::make_unique<MyGUI::delegates::Delegate<>>();
			auto token = std::make_shared<int>(42);
			std::weak_ptr<int> lifetime = token;
			std::array<int, 128> payload{};
			payload[100] = 7;
			int result = 0;
			*delegate = MyGUI::newDelegate(
				[&, token, payload]()
				{
					if (mutation == 0)
						delegate->clear();
					else if (mutation == 1)
						*delegate = MyGUI::newDelegate([]() {}, 2);
					else
						delegate.reset();
					require(!lifetime.expired(), "The executing single callback must retain its captures");
					result = *token + payload[100];
				},
				1);
			token.reset();
			(*delegate)();
			require(result == 49 && lifetime.expired(), "Single callback captures must be released after return");
		}
	}

	void testRemovedLambdaLifetime()
	{
		for (bool clearAll : {false, true})
		{
			Event event;
			auto token = std::make_shared<int>(42);
			std::weak_ptr<int> lifetime = token;
			std::array<int, 128> payload{};
			payload[100] = 7;
			int result = 0;
			event += MyGUI::newDelegate(
				[&, token, payload]()
				{
					if (clearAll)
						event.clear();
					else
						event -= MyGUI::newDelegate([]() {}, 1);
					require(event.empty(), "Removal must become visible before the callback returns");
					event();
					require(!lifetime.expired(), "Nested dispatch must not release an executing callback");
					result = *token + payload[100];
				},
				1);
			token.reset();
			event();
			require(result == 49 && lifetime.expired(), "Removed lambda captures must survive through return");
		}
	}

	void testDestroyEventDuringDispatch()
	{
		for (bool nested : {false, true})
		{
			auto event = std::make_unique<Event>();
			auto token = std::make_shared<int>(42);
			std::weak_ptr<int> lifetime = token;
			bool recursing = false;
			int result = 0, tailCalls = 0;
			*event += MyGUI::newDelegate(
				[&, token]()
				{
					if (nested && !recursing)
					{
						recursing = true;
						(*event)();
					}
					else
						event.reset();
					require(!lifetime.expired(), "Event destruction must retain all executing callbacks");
					result += *token;
				},
				1);
			*event += MyGUI::newDelegate([&tailCalls]() { ++tailCalls; }, 2);
			token.reset();
			(*event)();
			require(!event && tailCalls == 0, "Destroying an event must cancel all pending callbacks");
			require(result == (nested ? 84 : 42) && lifetime.expired(), "Dispatch must release its retained state");
		}
	}

	void testDestroyEventBeforeException()
	{
		auto event = std::make_unique<Event>();
		auto token = std::make_shared<int>(42);
		std::weak_ptr<int> lifetime = token;
		*event += MyGUI::newDelegate(
			[&, token]()
			{
				event.reset();
				require(*token == 42 && !lifetime.expired(), "Captures must survive event destruction before throwing");
				throw std::runtime_error("destroyed event");
			},
			1);
		token.reset();
		unittest::requireThrows<std::runtime_error>([&] { (*event)(); }, "Destroyed event exceptions must propagate");
		require(!event && lifetime.expired(), "Exceptional dispatch must release retained captures and state");
	}

	void testIndirectEventDestruction()
	{
		struct Owner
		{
			Event event;
		};
		// Separate storage lets ASan detect stale event accesses even while the weak_ptr survives.
		auto owner = std::shared_ptr<Owner>(new Owner);
		std::weak_ptr<Owner> lifetime = owner;
		owner->event += MyGUI::newDelegate([owner]() { owner->event -= MyGUI::newDelegate([]() {}, 1); }, 1);
		auto* event = &owner->event;
		owner.reset();
		(*event)();
		require(lifetime.expired(), "Releasing a removed capture may destroy the event during cleanup");
	}

	void testCaptureDestructorReentrancy()
	{
		for (bool duringDispatch : {false, true})
		{
			Event event;
			int calls = 0;
			auto token = std::shared_ptr<int>(
				new int(42),
				[&](int* value)
				{
					delete value;
					event.clear();
					event += MyGUI::newDelegate([&calls]() { ++calls; }, 3);
				});
			event += MyGUI::newDelegate([token]() {}, 1);
			event += MyGUI::newDelegate([&]() { event -= MyGUI::newDelegate([]() {}, 1); }, 2);
			token.reset();
			if (duringDispatch)
				event();
			else
				event.clear();
			event();
			require(calls == 1, "Capture destructors must be able to clear and resubscribe after removal");
		}
	}

	template<typename EventType>
	void subscribeDuringTeardown(EventType& event, typename EventType::IDelegate* callback)
	{
		if constexpr (std::is_same_v<EventType, Event>)
			event += callback;
		else
			event = callback;
	}

	template<typename EventType>
	void testCaptureDestructorDuringTeardown()
	{
		std::weak_ptr<int> replacementLifetime;
		bool firstReleased = false;
		bool replacementReleased = false;
		bool stayedEmpty = false;
		int calls = 0;
		{
			EventType event;
			auto token = std::shared_ptr<int>(
				new int(1),
				[&](int* value)
				{
					delete value;
					firstReleased = true;
					event.clear();
					auto replacement = std::shared_ptr<int>(
						new int(2),
						[&](int* replacementValue)
						{
							delete replacementValue;
							replacementReleased = true;
							event.clear();
						});
					replacementLifetime = replacement;
					subscribeDuringTeardown(event, MyGUI::newDelegate([replacement, &calls]() { ++calls; }, 2));
					stayedEmpty = event.empty();
					event();
				});
			subscribeDuringTeardown(event, MyGUI::newDelegate([token]() {}, 1));
		}
		require(firstReleased, "Event destruction must release its original captures");
		require(stayedEmpty && calls == 0, "Subscriptions during teardown must not resurrect or invoke the event");
		require(
			replacementReleased && replacementLifetime.expired(),
			"Callbacks subscribed by capture destructors during teardown must be released safely");
	}

	void testIndirectDestructionDuringAssignment()
	{
		// Keep event storage separate: an observing weak_ptr must not mask access after destruction.
		auto owner = std::shared_ptr<Event>(new Event);
		std::weak_ptr<Event> lifetime = owner;
		*owner += MyGUI::newDelegate([owner]() {}, 1);
		auto* event = owner.get();
		owner.reset();
		auto token = std::make_shared<int>(42);
		std::weak_ptr<int> replacementLifetime = token;

		// The obsolete replacement API still has to handle destruction while clearing the old callback.
		MYGUI_SUPPRESS_MSVC(4996)
		MYGUI_SUPPRESS_GCC("-Wdeprecated-declarations")
		*event = MyGUI::newDelegate([token]() {}, 2);
		MYGUI_UNSUPPRESS_GCC()
		MYGUI_UNSUPPRESS_MSVC()

		token.reset();
		require(lifetime.expired(), "Replacing the last owning callback may destroy the event");
		require(replacementLifetime.expired(), "Replacement during event destruction must not leak captures");
	}

	void testGrowthDuringDispatch()
	{
		Event event;
		std::vector<int> calls;
		event += MyGUI::newDelegate(
			[&]()
			{
				event -= MyGUI::newDelegate([]() {}, 1);
				for (int i = 0; i < 128; ++i)
					event += MyGUI::newDelegate([&, i]() { calls.push_back(i); }, i + 2);
			},
			1);
		for (int emission = 0; emission < 2; ++emission)
		{
			calls.clear();
			event();
			require(calls.size() == 128, "Appended listeners must run in the current emission and remain subscribed");
			for (int i = 0; i < 128; ++i)
				require(calls[static_cast<size_t>(i)] == i, "Growth must preserve connection order");
		}
	}

	void testMulticastValueArguments()
	{
		MyGUI::delegates::MultiDelegate<std::string, int&> event;
		std::vector<std::string> values;
		event += MyGUI::newDelegate(
			[&](std::string value, int& count)
			{
				values.push_back(std::move(value));
				++count;
			},
			1);
		event += MyGUI::newDelegate(
			[&](std::string value, int& count)
			{
				values.push_back(std::move(value));
				++count;
			},
			2);
		int calls = 0;
		event("intact argument", calls);
		require(
			calls == 2 && values == std::vector<std::string>({"intact argument", "intact argument"}),
			"Each multicast listener must receive an intact value and the same reference argument");
	}

	void testSingleMoveDuringDispatch()
	{
		using Single = MyGUI::delegates::Delegate<>;
		auto source = std::make_unique<Single>();
		Single destination;
		auto token = std::make_shared<int>(42);
		std::weak_ptr<int> lifetime = token;
		int result = 0;
		*source = MyGUI::newDelegate(
			[&, token]()
			{
				destination = std::move(*source);
				require(source->empty(), "Moving must leave the source delegate empty");
				source.reset();
				destination.clear();
				result = *token;
			},
			1);
		token.reset();
		(*source)();
		require(result == 42 && lifetime.expired(), "Moving and clearing must retain an executing callable");
		destination = MyGUI::newDelegate([&]() { ++result; }, 2);
		Single moved(std::move(destination));
		moved();
		require(destination.empty() && result == 43, "A moved delegate must remain invokable");
	}

	void testMultiMoveDuringDispatch()
	{
		for (bool clearMoved : {false, true})
		{
			auto source = std::make_unique<Event>();
			std::unique_ptr<Event> destination;
			int calls = 0;
			*source += MyGUI::newDelegate(
				[&]()
				{
					destination = std::make_unique<Event>(std::move(*source));
					require(source->empty(), "Moving an event must leave the source empty");
					source.reset();
					if (clearMoved)
						destination->clear();
				},
				1);
			*source += MyGUI::newDelegate([&]() { ++calls; }, 2);
			(*source)();
			require(calls == (clearMoved ? 0 : 1), "Active dispatch must follow the state transferred by a move");
			destination->clear();
			*destination += MyGUI::newDelegate([&]() { ++calls; }, 3);
			(*destination)();
			require(calls == (clearMoved ? 1 : 2), "The moved event must support clearing and reuse");
		}
	}

	void testReentrantCleanupDrainsRemovals()
	{
		for (bool destroyOwner : {false, true})
		{
			auto event = std::make_unique<Event>();
			std::weak_ptr<int> lifetime;
			auto first = std::shared_ptr<int>(
				new int(1),
				[&](int* value)
				{
					delete value;
					auto second = std::make_shared<int>(2);
					lifetime = second;
					*event += MyGUI::newDelegate([second]() {}, 2);
					if (destroyOwner)
						event.reset();
					else
						event->clear();
				});
			*event += MyGUI::newDelegate([first]() {}, 1);
			first.reset();
			event->clear();
			require(lifetime.expired(), "Cleanup must release callbacks removed by another capture destructor");
		}
	}

	void testRepeatedOriginalPointerRemoval()
	{
		Event event;
		Event::IDelegate* callback = nullptr;
		int calls = 0;
		callback = MyGUI::newDelegate(
			[&]()
			{
				event -= callback;
				event -= callback;
				event();
				++calls;
			},
			1);
		event += callback;
		event();
		require(calls == 1 && event.empty(), "An executing original pointer must not be destroyed by repeated removal");
	}

	void testMoveIntoDyingSingleDelegate()
	{
		using Single = MyGUI::delegates::Delegate<>;
		Single source;
		auto destination = std::make_unique<Single>();
		auto* dying = destination.get();
		auto moveOnDestruction = std::shared_ptr<int>(
			new int(0),
			[&](int* value)
			{
				delete value;
				*dying = std::move(source);
			});
		*destination = MyGUI::newDelegate([moveOnDestruction]() {}, 1);
		moveOnDestruction.reset();
		auto token = std::make_shared<int>(42);
		std::weak_ptr<int> lifetime = token;
		int result = 0;
		source = MyGUI::newDelegate(
			[&, token]()
			{
				destination.reset();
				result = *token;
			},
			2);
		token.reset();
		source();
		require(
			result == 42 && source.empty() && lifetime.expired(),
			"Discarding a move into a dying delegate must still retain the executing callback");
	}

	void testOriginalPointerRemovalDuringCaptureDestruction()
	{
		for (bool teardown : {false, true})
		{
			auto event = std::make_unique<Event>();
			auto* target = event.get();
			Event::IDelegate* callback = nullptr;
			bool released = false;
			auto token = std::shared_ptr<int>(
				new int(0),
				[&](int* value)
				{
					delete value;
					*target -= callback;
					released = true;
				});
			callback = MyGUI::newDelegate([token]() {}, 1);
			*event += callback;
			token.reset();
			if (teardown)
				event.reset();
			else
				event->clear();
			require(released, "Removing the original pointer during capture destruction must not destroy it twice");
		}
	}

	void testOriginalPointerRemovalFromMovedSource()
	{
		Event source;
		int calls = 0;
		auto* callback = MyGUI::newDelegate([&]() { ++calls; }, 1);
		source += callback;
		Event destination(std::move(source));
		source -= callback;
		destination();
		require(calls == 1, "Removal from a moved source must not delete a callback owned by the destination");
	}

	void testSingleMoveDestroysDestination()
	{
		using Single = MyGUI::delegates::Delegate<>;
		Single source;
		auto destination = std::shared_ptr<Single>(new Single);
		std::weak_ptr<Single> destinationLifetime = destination;
		*destination = MyGUI::newDelegate([destination]() {}, 1);
		auto* target = destination.get();
		destination.reset();
		auto token = std::make_shared<int>(42);
		std::weak_ptr<int> callbackLifetime = token;
		source = MyGUI::newDelegate([token]() {}, 2);
		token.reset();
		*target = std::move(source);
		require(
			destinationLifetime.expired() && callbackLifetime.expired() && source.empty(),
			"Move assignment must tolerate the old capture destroying its destination");
	}

	void testEmptyCallableInvocation()
	{
		MyGUI::delegates::Delegate<> single;
		void (*function)() = nullptr;
		single = MyGUI::newDelegate(function);
		unittest::requireThrows<std::bad_function_call>(
			[&]() { single(); },
			"Null function pointers remain empty callables");
		single = MyGUI::newDelegate(std::function<void()>{}, 1);
		unittest::requireThrows<std::bad_function_call>(
			[&]() { single(); },
			"Empty std::function must propagate its exception");
		Event event;
		event += MyGUI::newDelegate(function);
		unittest::requireThrows<std::bad_function_call>(
			[&]() { event(); },
			"Multicast must propagate empty callable exceptions");
		event -= MyGUI::newDelegate(function);
		require(event.empty(), "An empty callable must remain removable by identity");
	}

	struct UnlinkingReceiver : MyGUI::delegates::IDelegateUnlink
	{
		Event event;
		bool nested{false};
		bool removeByOwner{false};
		int calls{0};

		void recurseAndRemove()
		{
			++calls;
			if (!nested)
			{
				nested = true;
				event();
				nested = false;
				if (removeByOwner)
					event.clear(this);
				else
					event -= MyGUI::newDelegate(this, &UnlinkingReceiver::recurseAndRemove);
			}
		}

		[[noreturn]] void removeAndThrow()
		{
			event.clear(this);
			throw std::runtime_error("removed callback failure");
		}
	};

	void testRemovalAfterNestedDispatch()
	{
		for (bool removeByOwner : {false, true})
		{
			UnlinkingReceiver receiver;
			receiver.removeByOwner = removeByOwner;
			int tailCalls = 0;
			receiver.event += MyGUI::newDelegate(&receiver, &UnlinkingReceiver::recurseAndRemove);
			receiver.event += MyGUI::newDelegate([&tailCalls]() { ++tailCalls; }, 1);
			receiver.event();
			require(
				receiver.calls == 2 && tailCalls == 2,
				"Removing after nested dispatch must preserve the outer iterator and remaining callbacks");
			receiver.event();
			require(receiver.calls == 2 && tailCalls == 3, "The removed receiver must stay unsubscribed");
		}
	}

	void testUnlinkDuringException()
	{
		UnlinkingReceiver receiver;
		int tailCalls = 0;
		receiver.event += MyGUI::newDelegate(&receiver, &UnlinkingReceiver::removeAndThrow);
		receiver.event += MyGUI::newDelegate([&tailCalls]() { ++tailCalls; }, 1);
		unittest::requireThrows<std::runtime_error>(
			[&] { receiver.event(); },
			"Unlinking before an exception must preserve exception propagation");
		require(tailCalls == 0, "An exception must skip pending callbacks");
		receiver.event -= MyGUI::newDelegate([]() {}, 1);
		require(receiver.event.empty(), "Removal after exceptional dispatch must leave an empty event");
		receiver.event += MyGUI::newDelegate([&tailCalls]() { ++tailCalls; }, 2);
		receiver.event();
		require(tailCalls == 1, "The event must remain reusable after exceptional unlinking");
	}

}

int main()
{
	return unittest::runTests({
		{"Single delegate invocation and replacement", testSingleDelegate},
		{"Single delegate ownership", testSingleOwnership},
		{"Callback identity", testCallbackIdentity},
		{"Inherited member callbacks", testInheritedMemberCallbacks},
		{"Function IDs and invocation order", testFunctionIdsAndOrder},
		{"Duplicate subscription", testDuplicateSubscription},
		{"Empty events and removal", testEmptyAndRemoval},
		{"Receiver unlinking", testUnlinkReceiver},
		{"Multi delegate ownership", testMultiOwnership},
		{"Self-removal", testSelfRemoval},
		{"Remove a pending listener", testRemovePendingListener},
		{"Clear during dispatch", testClearDuringDispatch},
		{"Nested dispatch", testNestedDispatch},
		{"Clear after nested dispatch", testClearAfterNestedDispatch},
		{"Exception recovery", testExceptionRecovery},
		{"Removal after nested dispatch", testRemovalAfterNestedDispatch},
		{"Unlink during exceptional dispatch", testUnlinkDuringException},
		{"Single callback lifetime during mutation", testSingleCallbackLifetime},
		{"Removed lambda capture lifetime", testRemovedLambdaLifetime},
		{"Destroy event during nested dispatch", testDestroyEventDuringDispatch},
		{"Destroy event before exception", testDestroyEventBeforeException},
		{"Indirect event destruction during cleanup", testIndirectEventDestruction},
		{"Capture destructor reentrancy", testCaptureDestructorReentrancy},
		{"Single delegate capture reentry during teardown",
		 testCaptureDestructorDuringTeardown<MyGUI::delegates::Delegate<>>},
		{"Multi delegate capture reentry during teardown", testCaptureDestructorDuringTeardown<Event>},
		{"Indirect event destruction during obsolete assignment", testIndirectDestructionDuringAssignment},
		{"Storage growth during dispatch", testGrowthDuringDispatch},
		{"Multicast value and reference arguments", testMulticastValueArguments},
		{"Single delegate move during dispatch", testSingleMoveDuringDispatch},
		{"Multi delegate move during dispatch", testMultiMoveDuringDispatch},
		{"Reentrant cleanup drains additional removals", testReentrantCleanupDrainsRemovals},
		{"Repeated removal of the executing original pointer", testRepeatedOriginalPointerRemoval},
		{"Move into a single delegate during teardown", testMoveIntoDyingSingleDelegate},
		{"Original pointer removal during capture destruction", testOriginalPointerRemovalDuringCaptureDestruction},
		{"Original pointer removal from a moved source", testOriginalPointerRemovalFromMovedSource},
		{"Single move assignment destroys its destination", testSingleMoveDestroysDestination},
		{"Empty callable exception behavior", testEmptyCallableInvocation},
	});
}
