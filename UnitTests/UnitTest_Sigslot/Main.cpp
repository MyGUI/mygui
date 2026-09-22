#include "sigslot.h"
#include "BehaviourTestSupport.h"

namespace
{

	using unittest::require;
	using Event = sigslot::signal0<>;

	struct Receiver : sigslot::has_slots<>
	{
		std::function<void()> onCall = []() {};
		int calls = 0;

		void run()
		{
			++calls;
			// A callback may delete this receiver, so retain the callable locally.
			auto callback = onCall;
			callback();
		}

		void other()
		{
			calls += 10;
		}

		void add(int& _value) const
		{
			++_value;
		}
	};

	void testUniqueConnections()
	{
		Event event;
		Event otherEvent;
		Receiver first;
		Receiver second;
		std::vector<int> calls;
		first.onCall = [&calls]()
		{
			calls.push_back(1);
		};
		second.onCall = [&calls]()
		{
			calls.push_back(2);
		};
		require(event.connect_unique(&first, &Receiver::run), "The first registration must add a connection");
		require(event.connect_unique(&second, &Receiver::run), "Different receivers must have distinct connections");
		require(
			!event.connect_unique(&first, &Receiver::run),
			"Repeated registration must preserve the existing connection");
		event();
		require(calls == std::vector<int>({1, 2}), "Repeated registration must not duplicate or reorder callbacks");
		require(
			event.connect_unique(&first, &Receiver::other),
			"Different methods on one receiver must remain distinct");
		require(otherEvent.connect_unique(&first, &Receiver::run), "Uniqueness must be local to each signal");

		event.disconnect(&first);
		require(event.connect_unique(&first, &Receiver::run), "A disconnected receiver must be reconnectable");
		calls.clear();
		event();
		require(calls == std::vector<int>({2, 1}), "Reconnection must append a new connection");
		first.disconnect_all();
		require(event.connect_unique(&first, &Receiver::run), "Receiver-side disconnection must permit reconnection");
		require(
			otherEvent.connect_unique(&first, &Receiver::run),
			"Receiver-side disconnection must affect every signal");
		event.disconnect_all();
		event.connect(&first, &Receiver::run);
		event.connect(&first, &Receiver::run);
		require(
			!event.connect_unique(&first, &Receiver::run),
			"Unique registration must also detect ordinary connections");
		calls.clear();
		event();
		require(calls == std::vector<int>({1, 1}), "Unique registration must not remove existing ordinary duplicates");

		struct Derived : Receiver
		{
		};
		const Derived receiver;
		sigslot::signal<int&> reference;
		require(
			reference.connect_unique(&receiver, &Receiver::add),
			"Unique registration must accept inherited const methods");
		require(
			!reference.connect_unique(static_cast<const Receiver*>(&receiver), &Receiver::add),
			"Receiver identity must be preserved when using a base-class pointer");
		int value = 0;
		reference(value);
		require(value == 1, "An inherited const callback must run exactly once");
	}

	void testUniqueConnectionsDuringEmission()
	{
		Event event;
		Receiver first;
		Receiver added;
		std::vector<int> calls;
		bool nested = false;
		first.onCall = [&]()
		{
			calls.push_back(1);
			if (!nested)
			{
				nested = true;
				require(
					event.connect_unique(&added, &Receiver::run),
					"Registration during emission must add new callbacks");
				require(
					!event.connect_unique(&added, &Receiver::run),
					"New callbacks must immediately count as connected");
				event();
			}
		};
		added.onCall = [&calls]()
		{
			calls.push_back(2);
		};
		event.connect(&first, &Receiver::run);
		event();
		require(calls == std::vector<int>({1, 1, 2}), "Unique callbacks must follow nested emission snapshot rules");

		first.onCall = [&]()
		{
			event.disconnect(&added);
			require(
				event.connect_unique(&added, &Receiver::run),
				"A pending disconnected callback must be reconnectable");
		};
		calls.clear();
		event();
		require(calls.empty(), "Reconnection must skip the old pending slot and defer the replacement");
		event.disconnect(&first);
		event();
		require(calls == std::vector<int>({2}), "The replacement callback must run once on the next emission");
	}

}

int main()
{
	return unittest::runTests({
		{"Unique connections and reconnection", testUniqueConnections},
		{"Unique connections during emission", testUniqueConnectionsDuringEmission},
	});
}
