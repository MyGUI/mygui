#include "sigslot.h"
#include "TestRunner.h"
#include "TestSupport.h"
#include <stdexcept>
#include <string>

namespace
{

	using unittest::require;
	using Event = sigslot::signal0<>;

	template<typename... Args>
	struct Receiver : sigslot::has_slots<>
	{
		std::function<void(Args...)> callback = [](Args...) {};

		void run(Args... args) const
		{
			auto invoke = callback; // The callback may destroy this receiver.
			invoke(args...);
		}

		void other(Args... args) const
		{
			run(args...);
		}
	};

	void testConnections()
	{
		Event event, other;
		Receiver<> first, second;
		std::vector<int> calls;
		first.callback = [&]()
		{
			calls.push_back(1);
		};
		second.callback = [&]()
		{
			calls.push_back(2);
		};
		event();
		require(!event.exist(&first, &Receiver<>::run), "Empty signal");
		require(event.connect_unique(&first, &Receiver<>::run), "First connection");
		require(event.connect_unique(&second, &Receiver<>::run), "Distinct receiver");
		require(!event.connect_unique(&first, &Receiver<>::run), "Duplicate rejected");
		require(event.connect_unique(&first, &Receiver<>::other), "Distinct method");
		event.connect(&first, &Receiver<>::run);
		event.emit();
		require(calls == std::vector<int>({1, 2, 1, 1}), "Order and ordinary duplicates preserved");
		event.disconnect(&first);
		require(!event.exist(&first, &Receiver<>::other), "All receiver connections removed");
		require(event.connect_unique(&first, &Receiver<>::run), "Reconnect after removal");
		require(other.connect_unique(&first, &Receiver<>::run), "Uniqueness is per signal");
		first.disconnect_all();
		first.disconnect_all();
		calls.clear();
		event();
		other();
		require(calls == std::vector<int>({2}), "Receiver disconnection affects every signal");
		require(event.connect_unique(&first, &Receiver<>::run), "Reconnect after receiver disconnection");
	}

	template<typename Signal, typename... Args>
	void checkArity(Args... args)
	{
		Signal event;
		int sum = 0;
		event.connect([&](auto... values) { sum = (values + ...); });
		event(args...);
		require(sum == 6, "All arguments delivered");
	}

	void testArguments()
	{
		struct Derived : Receiver<int&>
		{
		};
		Derived receiver;
		receiver.callback = [](int& value)
		{
			++value;
		};
		sigslot::signal1<int&> event;
		require(event.connect_unique(&receiver, &Receiver<int&>::run), "Inherited const method");
		require(
			!event.connect_unique(static_cast<const Receiver<int&>*>(&receiver), &Receiver<int&>::run),
			"Base pointer has the same identity");
		event.connect(+[](int& value) { ++value; });
		int value = 0;
		event(value);
		require(value == 2, "Reference arguments and free functions");

		sigslot::signal2<std::string, bool&> command;
		command.connect([](std::string name, bool& handled) { handled = name == "save"; });
		command.connect([](std::string name, bool& handled)
						{ require(handled && name == "save", "Intact multicast value"); });
		bool handled = false;
		command("save", handled);
		checkArity<sigslot::signal3<int, int, int>>(1, 2, 3);
		checkArity<sigslot::signal4<int, int, int, int>>(1, 2, 3, 0);
		checkArity<sigslot::signal5<int, int, int, int, int>>(1, 2, 3, 0, 0);
		checkArity<sigslot::signal<int, int, int, int, int, int, int, int, int>>(1, 2, 3, 0, 0, 0, 0, 0, 0);
	}

	void testLifetimes()
	{
		Event event, other;
		int calls = 0;
		{
			Receiver<> receiver;
			receiver.callback = [&]()
			{
				++calls;
			};
			event.connect(&receiver, &Receiver<>::run);
			event.connect(&receiver, &Receiver<>::run);
			other.connect(&receiver, &Receiver<>::run);
			event();
		}
		event();
		other();
		require(calls == 2, "Destroyed receivers disconnect everywhere");
		Receiver<> survivor;
		{
			Event temporary;
			temporary.connect(&survivor, &Receiver<>::run);
		}
		survivor.disconnect_all();

		auto owner = std::make_shared<int>(7);
		std::weak_ptr<int> lifetime = owner;
		sigslot::signal<std::shared_ptr<int>> property;
		Receiver<const std::shared_ptr<int>&> receiver;
		receiver.callback = [&](const auto& value)
		{
			owner.reset();
			require(*value == 7, "Argument stays alive");
		};
		property.connect(&receiver, &Receiver<const std::shared_ptr<int>&>::run);
		property(owner);
		require(lifetime.expired(), "Argument released after emission");
	}

	void testNestedEmission()
	{
		Event event;
		Receiver<> added;
		std::vector<int> calls;
		bool nested = false;
		added.callback = [&]()
		{
			calls.push_back(3);
		};
		event.connect(
			[&]()
			{
				calls.push_back(1);
				if (!nested)
				{
					nested = true;
					require(event.connect_unique(&added, &Receiver<>::run), "Add during emission");
					require(!event.connect_unique(&added, &Receiver<>::run), "New connection already visible");
					event();
				}
			});
		event.connect([&]() { calls.push_back(2); });
		event();
		require(calls == std::vector<int>({1, 1, 2, 3, 2}), "Only nested emission sees the added slot");
		calls.clear();
		event();
		require(calls == std::vector<int>({1, 2, 3}), "Later emissions preserve order");
	}

	void testMutationDuringEmission()
	{
		enum class Mutation
		{
			Disconnect,
			DestroyReceivers,
			Clear,
			DestroySignal
		};
		for (auto mutation :
			 {Mutation::Disconnect, Mutation::DestroyReceivers, Mutation::Clear, Mutation::DestroySignal})
		{
			auto event = std::make_unique<Event>();
			auto first = std::make_unique<Receiver<>>();
			auto pending = std::make_unique<Receiver<>>();
			int pendingCalls = 0, tailCalls = 0;
			pending->callback = [&]()
			{
				++pendingCalls;
			};
			first->callback = [&]()
			{
				switch (mutation)
				{
				case Mutation::Disconnect:
					event->disconnect(first.get());
					event->disconnect(pending.get());
					require(event->connect_unique(pending.get(), &Receiver<>::run), "Reconnect pending slot");
					break;
				case Mutation::DestroyReceivers:
					first.reset();
					pending.reset();
					break;
				case Mutation::Clear:
					event->disconnect_all();
					event->connect(pending.get(), &Receiver<>::run);
					break;
				case Mutation::DestroySignal: event.reset(); break;
				}
			};
			event->connect(first.get(), &Receiver<>::run);
			event->connect(pending.get(), &Receiver<>::run);
			event->connect([&]() { ++tailCalls; });
			(*event)();
			bool keepsTail = mutation == Mutation::Disconnect || mutation == Mutation::DestroyReceivers;
			require(pendingCalls == 0 && tailCalls == (keepsTail ? 1 : 0), "Pending callbacks respect removal");
			if (event)
			{
				(*event)();
				require(pendingCalls == (pending ? 1 : 0), "Replacement runs on the next emission");
			}
		}
	}

	void testExceptionsAndCaptureOwnership()
	{
		Event event;
		Receiver<> throwing;
		int calls = 0;
		throwing.callback = [&]()
		{
			if (calls == 0)
				throw std::runtime_error("callback failure");
		};
		event.connect(&throwing, &Receiver<>::run);
		auto capture = std::make_shared<int>(0);
		std::weak_ptr<int> lifetime = capture;
		event.connect([&, capture]() { calls = ++*capture; });
		capture.reset();
		bool caught = false;
		try
		{
			event();
		}
		catch (const std::runtime_error&)
		{
			caught = true;
		}
		require(caught && calls == 0 && !lifetime.expired(), "Exception stops dispatch without dropping captures");
		event.disconnect(&throwing);
		event();
		require(calls == 1, "Dispatch recovers after an exception");
		event.disconnect_all();
		require(lifetime.expired(), "Clearing releases captures");
	}

}

int main()
{
	return unittest::runTests({
		{"Connection identity and uniqueness", testConnections},
		{"Arguments and variadic arities", testArguments},
		{"Sender, receiver and argument lifetimes", testLifetimes},
		{"Nested emission", testNestedEmission},
		{"Removal and destruction during emission", testMutationDuringEmission},
		{"Exceptions and callable ownership", testExceptionsAndCaptureOwnership},
	});
}
