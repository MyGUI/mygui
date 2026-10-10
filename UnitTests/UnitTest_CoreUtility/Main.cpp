#include "MyGUI_StringUtility.h"
#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_BackwardCompatibility.h"
#include "MyGUI_ILogListener.h"
#include <any>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <locale>
#include <memory>
#include <stdexcept>
#include <vector>

namespace
{

	using unittest::require;

	struct CastSource : MyGUI::IObject
	{
		MYGUI_RTTI_DERIVED(CastSource)
	};

	struct CastTarget : MyGUI::IObject
	{
		MYGUI_RTTI_DERIVED(CastTarget)
	};

	class CapturedLog : public MyGUI::ILogListener
	{
	public:
		void log(
			std::string_view _section,
			MyGUI::LogLevel _level,
			const tm*,
			std::string_view _message,
			std::string_view _file,
			int _line) override
		{
			++count;
			section = _section;
			level = _level;
			message = _message;
			file = _file;
			line = _line;
		}

		int count{};
		std::string section, message, file;
		MyGUI::LogLevel level;
		int line{};
	};

	template<typename Action>
	void checkCastError(Action _action, CapturedLog& _log, const std::string& _message, std::string_view _file)
	{
		const int before = _log.count;
		try
		{
			_action();
		}
		catch (const MyGUI::Exception& error)
		{
			require(error.getDescription() == _message + "\n", "Cast exception text must be preserved");
			require(error.getSource() == "MyGUI", "Cast exception source must be preserved");
			require(
				error.getFile().find(_file) != std::string::npos && error.getLine() > 0,
				"Cast exceptions must point to the template, not the shared helper");
			require(
				_log.count == before + 1 && _log.message == _message,
				"A throwing cast must log exactly once without the exception newline");
			require(
				_log.section == "Core" && _log.level == MyGUI::LogLevel::Critical,
				"Cast logging must keep its section and severity");
			require(
				_log.file == error.getFile() && _log.line == error.getLine(),
				"Log and exception locations must match");
			return;
		}
		require(false, "A mismatched throwing cast must throw MyGUI::Exception");
	}

	void testCasts()
	{
		CapturedLog captured;
		MyGUI::LogSource source;
		source.addLogListener(&captured);
		MyGUI::LogManager logs;
		logs.addLogSource(&source);

		CastSource object;
		CastTarget target;
		require(
			object.isType<CastSource>() && object.isType<MyGUI::IObject>() && !object.isType<CastTarget>(),
			"Source type checks must recognize its own type and base, but reject the unrelated target");
		require(
			target.isType<CastTarget>() && target.isType<MyGUI::IObject>() && !target.isType<CastSource>(),
			"Target type checks must recognize its own type and base, but reject the unrelated source");
		MyGUI::IObject& base = object;
		const MyGUI::IObject& constant = object;
		require(
			base.castType<CastSource>() == &object && constant.castType<CastSource>() == &object,
			"Mutable and const casts must preserve the object");
		require(
			base.castType<CastTarget>(false) == nullptr && constant.castType<CastTarget>(false) == nullptr,
			"Nonthrowing mismatched casts must return null");
		require(captured.count == 0, "Successful and nonthrowing casts must not log");
		const std::string message = "Cannot cast from type 'CastSource' to type 'CastTarget'.";
		checkCastError([&] { base.castType<CastTarget>(); }, captured, message, "MyGUI_IObject.h");
		checkCastError([&] { constant.castType<CastTarget>(); }, captured, message, "MyGUI_IObject.h");

		const MyGUI::Any value(42);
		const MyGUI::Any empty;
		require(
			*value.castType<int>() == 42 && value.castType<float>(false) == nullptr &&
				empty.castType<int>(false) == nullptr,
			"Any casts must preserve value and empty-state behavior");
		require(captured.count == 2, "Nonthrowing Any casts must not log");
		checkCastError(
			[&] { value.castType<float>(); },
			captured,
			std::string("Cannot cast from type '") + typeid(int).name() + "' to type '" + typeid(float).name() + "'.",
			"MyGUI_Any.h");
		checkCastError(
			[&] { empty.castType<int>(); },
			captured,
			std::string("Cannot cast from type '") + typeid(void).name() + "' to type '" + typeid(int).name() + "'.",
			"MyGUI_Any.h");
		MyGUI::Any mutableValue(42);
		require(
			mutableValue.castType<float>(false) == nullptr,
			"Mutable nonthrowing mismatched casts must return null");
		checkCastError(
			[&] { mutableValue.castType<float>(); },
			captured,
			std::string("Cannot cast from type '") + typeid(int).name() + "' to type '" + typeid(float).name() + "'.",
			"MyGUI_Any.h");
	}

	struct NonComparable
	{
		int value = 7;
	};

	struct ThrowingCopy
	{
		bool fail = false;

		ThrowingCopy() = default;
		ThrowingCopy(const ThrowingCopy& other) :
			fail(other.fail)
		{
			if (fail)
				throw std::runtime_error("Copy failed");
		}
	};

	struct ConstComparable
	{
		int value = 0;

		bool operator==(const ConstComparable& other) const&
		{
			return value == other.value;
		}
		bool operator==(const ConstComparable&) const&& = delete;
	};

	struct MutableComparable
	{
		bool operator==(const MutableComparable&)
		{
			return true;
		}
	};

	struct alignas(64) AlignedValue
	{
		int value = 42;
	};

	struct RestrictedAllocation
	{
		static void* operator new(std::size_t) = delete;
		static void operator delete(void*) = delete;

		int value = 42;
	};

	struct CountedValue
	{
		explicit CountedValue(int& count) :
			live(&count)
		{
			++*live;
		}

		CountedValue(const CountedValue& other) :
			live(other.live)
		{
			++*live;
		}

		~CountedValue()
		{
			--*live;
		}

		int* live;
	};

	void testAnyStorage()
	{
		MyGUI::Any empty;
		require(empty.empty() && empty.getType() == typeid(void), "Default Any must have no value or type");
		require(empty.compare(MyGUI::Any::Null), "Empty Any values must compare equal");

		MyGUI::Any value(42);
		const MyGUI::Any& constant = value;
		static_assert(std::is_same_v<decltype(value.castType<int>()), int*>);
		static_assert(std::is_same_v<decltype(constant.castType<int>()), const int*>);
		require(
			value.castType<const int>() == value.castType<int>() &&
				constant.castType<volatile int>() == constant.castType<int>(),
			"Casts may add const or volatile qualifiers to the stored type");
		require(
			value.castType<void>(false) == nullptr && value.castType<int[2]>(false) == nullptr &&
				value.castType<void()>(false) == nullptr && constant.castType<std::unique_ptr<int>>(false) == nullptr,
			"Queries for types that cannot be stored must return null");
		*value.castType<int>() = 43;
		require(*constant.castType<int>() == 43, "Const casts must observe changes made through mutable casts");
		MyGUI::Any copied(value);
		*copied.castType<int>() = 44;
		require(*value.castType<int>() == 43, "Copy construction must create an independent value");
		empty = value;
		*empty.castType<int>() = 45;
		require(*value.castType<int>() == 43, "Copy assignment must create an independent value");

		copied = std::string("replacement");
		require(copied.getType() == typeid(std::string), "Value assignment must replace the stored type");
		copied = *copied.castType<std::string>();
		require(
			copied.compare(MyGUI::Any(std::string("replacement"))),
			"Assigning the contained value must preserve it and its comparator");
		MyGUI::Any moved(std::move(copied));
		require(copied.empty() && copied.getType() == typeid(void), "Move construction must empty the source");
		require(moved.compare(MyGUI::Any(std::string("replacement"))), "Move construction must retain comparison");
		empty = std::move(moved);
		require(moved.empty(), "Move assignment must empty the source");
		require(empty.compare(MyGUI::Any(std::string("replacement"))), "Move assignment must retain comparison");

		const auto& self = empty;
		empty = self;
		auto& mutableSelf = empty;
		empty = std::move(mutableSelf);
		require(empty.compare(MyGUI::Any(std::string("replacement"))), "Self-assignment must preserve the value");
		empty = MyGUI::Any::Null;
		require(empty.empty(), "Assigning Null must clear the value");
		MyGUI::Any copiedEmpty(empty);
		MyGUI::Any movedEmpty(std::move(copiedEmpty));
		require(movedEmpty.empty() && copiedEmpty.empty(), "Copying and moving empty values must remain empty");
		copied = 12;
		require(copied.compare(MyGUI::Any(12)), "A moved-from Any must accept a new value and comparator");
		const MyGUI::Any nested(std::any(23));
		require(
			nested.getType() == typeid(std::any) && std::any_cast<int>(*nested.castType<std::any>()) == 23,
			"Storing std::any must preserve the container as the value");
		const MyGUI::Any text("text");
		const MyGUI::Any textCopy(text);
		require(text.getType() == typeid(const char*) && text.compare(textCopy), "Array decay must retain constness");
		const volatile int volatileValue = 42;
		const MyGUI::Any fromVolatile(volatileValue);
		require(fromVolatile.compare(MyGUI::Any(42)), "Construction must copy values from volatile sources");
		const MyGUI::Any restricted{RestrictedAllocation{}};
		const MyGUI::Any restrictedCopy(restricted);
		require(
			restrictedCopy.castType<RestrictedAllocation>()->value == 42,
			"Storing and copying values must not require class-specific allocation functions");
		const MyGUI::Any aligned{AlignedValue{}};
		const MyGUI::Any alignedCopy(aligned);
		require(alignedCopy.castType<AlignedValue>()->value == 42, "Over-aligned values must survive copying");
		require(
			reinterpret_cast<std::uintptr_t>(alignedCopy.castType<AlignedValue>()) % alignof(AlignedValue) == 0,
			"Stored values must retain their required alignment");

		int live = 0;
		{
			const CountedValue tracked(live);
			MyGUI::Any first(tracked);
			MyGUI::Any second(first);
			require(live == 3, "Each Any copy must own a separate value");
			MyGUI::Any transferred(std::move(first));
			second = std::move(transferred);
			require(live == 2, "Move assignment must release the replaced value without copying the incoming value");
			second = MyGUI::Any::Null;
			require(live == 1, "Clearing Any must destroy its stored value");
		}
		require(live == 0, "Every constructed value must be destroyed exactly once");

		MyGUI::Any source{ThrowingCopy{}};
		source.castType<ThrowingCopy>()->fail = true;
		MyGUI::Any destination(17);
		for (bool assignAny : {false, true})
		{
			bool threw = false;
			try
			{
				if (assignAny)
					destination = source;
				else
					destination = *source.castType<ThrowingCopy>();
			}
			catch (const std::runtime_error&)
			{
				threw = true;
			}
			require(threw, "Copy failures must propagate from both assignment overloads");
			require(destination.compare(MyGUI::Any(17)), "Failed assignment must preserve the value and comparator");
		}
	}

	void testAnyNestedAssignment()
	{
		struct Box
		{
			MyGUI::Any child;
		};
		for (const auto& expected : {MyGUI::Any{}, MyGUI::Any(42), MyGUI::Any(std::string(128, 'x'))})
		{
			MyGUI::Any copied(Box{expected});
			copied = copied.castType<Box>()->child;
			require(copied.compare(expected), "Copy assignment from a nested Any must retain its value and comparator");
			MyGUI::Any moved(Box{expected});
			moved = std::move(moved.castType<Box>()->child);
			require(moved.compare(expected), "Move assignment from a nested Any must retain its value and comparator");
		}

		MyGUI::Any value(Box{MyGUI::Any(ThrowingCopy{})});
		value.castType<Box>()->child.castType<ThrowingCopy>()->fail = true;
		unittest::requireThrows<std::runtime_error>(
			[&] { value = value.castType<Box>()->child; },
			"Copy failures must propagate when assigning from a nested Any");
		require(
			value.castType<Box>()->child.castType<ThrowingCopy>()->fail,
			"Failed nested assignment must leave the owning value intact");
	}

	void testAnyPointerStability()
	{
		std::vector<MyGUI::Any> values;
		values.emplace_back(42);
		int* saved = values.front().castType<int>();
		values.reserve(values.capacity() + 1);
		require(values.front().castType<int>() == saved, "Reallocation must preserve pointers to stored values");
		values.insert(values.begin(), MyGUI::Any(17));
		require(values[1].castType<int>() == saved, "Insertion must preserve pointers to shifted values");
		values.erase(values.begin());
		require(values.front().castType<int>() == saved, "Erasure must preserve pointers to surviving values");
		MyGUI::Any moved(std::move(values.front()));
		require(moved.castType<int>() == saved, "Move construction must preserve the stored value's address");
		MyGUI::Any assigned(0);
		assigned = std::move(moved);
		require(assigned.castType<int>() == saved, "Move assignment must preserve the incoming value's address");
		*saved = 43;
		require(assigned.compare(MyGUI::Any(43)), "Saved pointers must still modify the value after moves");
	}

	void testAnyComparison()
	{
		MyGUI::LogManager logs;
		const MyGUI::Any value(42);
		require(value.compare(MyGUI::Any(42)), "Equal values of the same type must compare equal");
		require(!value.compare(MyGUI::Any(43)), "Different values of the same type must compare unequal");
		require(!value.compare(MyGUI::Any(42.0)), "Different types must compare unequal even with equal values");
		const MyGUI::Any nan(std::numeric_limits<double>::quiet_NaN());
		require(!nan.compare(nan), "Self-comparison must use the stored value's equality operator");
		require(
			!value.compare(MyGUI::Any::Null) && !MyGUI::Any::Null.compare(value),
			"Empty and nonempty values must compare unequal in either order");
		using Pair = std::pair<int, std::string>;
		const MyGUI::Any pair(Pair{1, "one"});
		require(pair.compare(MyGUI::Any(Pair{1, "one"})), "Comparable pairs must compare their values");
		require(!pair.compare(MyGUI::Any(Pair{2, "one"})), "Comparable pairs must detect unequal values");
		using NestedPair = std::pair<int, Pair>;
		const MyGUI::Any nestedPair(NestedPair{1, {2, "two"}});
		require(
			nestedPair.compare(MyGUI::Any(NestedPair{1, {2, "two"}})) &&
				!nestedPair.compare(MyGUI::Any(NestedPair{1, {3, "two"}})),
			"Nested comparable pairs must compare their values");
		const MyGUI::Any constComparable(ConstComparable{7});
		require(
			constComparable.compare(MyGUI::Any(ConstComparable{7})) &&
				!constComparable.compare(MyGUI::Any(ConstComparable{8})),
			"Comparison must support equality operators restricted to const lvalues");

		const MyGUI::Any function(&testAnyStorage);
		require(function.compare(MyGUI::Any(&testAnyStorage)), "Equal function pointers must compare equal");
		require(!function.compare(MyGUI::Any(&testAnyComparison)), "Different function pointers must compare unequal");
		const MyGUI::Any member(&MyGUI::Any::empty);
		require(member.compare(MyGUI::Any(&MyGUI::Any::empty)), "Member function pointers must remain comparable");

		const MyGUI::Any nonComparable(NonComparable{});
		require(nonComparable.castType<NonComparable>()->value == 7, "Noncomparable values must still be storable");
		require(!nonComparable.compare(MyGUI::Any::Null), "Noncomparable values must compare unequal to empty");
		require(!value.compare(nonComparable), "A comparable left operand must reject a different type");
		const auto checkUnsupported = [](const MyGUI::Any& left, const MyGUI::Any& right)
		{
			bool threw = false;
			try
			{
				left.compare(right);
			}
			catch (const MyGUI::Exception& error)
			{
				threw = true;
				require(
					error.getDescription() == std::string("Type '") + left.getType().name() + "' is not comparable\n",
					"Unsupported comparison must preserve its exception text");
			}
			require(threw, "A noncomparable left operand must throw when the right operand is nonempty");
		};
		checkUnsupported(nonComparable, nonComparable);
		checkUnsupported(nonComparable, value);
		const MyGUI::Any nonComparablePair(std::pair<int, NonComparable>{});
		checkUnsupported(nonComparablePair, nonComparablePair);
		const MyGUI::Any nestedNonComparablePair(std::pair<int, std::pair<int, NonComparable>>{});
		checkUnsupported(nestedNonComparablePair, nestedNonComparablePair);
		MutableComparable mutableValue;
		require(mutableValue == MutableComparable{}, "The mutable-only test type must support nonconst comparison");
		const MyGUI::Any mutableComparable(mutableValue);
		checkUnsupported(mutableComparable, mutableComparable);
		const MyGUI::Any mutableComparablePair(std::pair<int, MutableComparable>{});
		checkUnsupported(mutableComparablePair, mutableComparablePair);
	}

	void testUserDataConstAccess()
	{
		MyGUI::UserData data;
		const MyGUI::UserData& constant = data;
		static_assert(std::is_same_v<decltype(data.getUserData<int>()), int*>);
		static_assert(std::is_same_v<decltype(constant.getUserData<int>()), const int*>);
		static_assert(std::is_same_v<decltype(data._getInternalData<int>()), int*>);
		static_assert(std::is_same_v<decltype(constant._getInternalData<int>()), const int*>);
		data.setUserData(1);
		*data.getUserData<int>() = 2;
		require(
			*constant.getUserData<int>() == 2,
			"Const user-data access must observe changes through mutable access");
		data._setInternalData(3);
		*data._getInternalData<int>() = 4;
		require(*constant._getInternalData<int>() == 4, "Const internal-data access must observe mutable changes");
		require(
			constant.getUserData<float>(false) == nullptr && constant._getInternalData<float>(false) == nullptr,
			"Const data getters must preserve nonthrowing mismatched casts");
	}

	template<typename T>
	void checkNumericParser()
	{
		using MyGUI::utility::parseValue;
		require(parseValue<T>(" \t+42\n") == T(42), "Numeric parsing must accept leading/trailing whitespace and plus");
		for (auto input : {"", "abc", "42x", "42 9"})
			require(parseValue<T>(input) == T{}, "Invalid or trailing numeric input must return the default value");
	}

	void testParsing()
	{
		using MyGUI::utility::parseValue;
		checkNumericParser<short>();
		checkNumericParser<unsigned short>();
		checkNumericParser<int>();
		checkNumericParser<unsigned int>();
		checkNumericParser<long>();
		checkNumericParser<unsigned long>();
		checkNumericParser<long long>();
		checkNumericParser<unsigned long long>();
		checkNumericParser<float>();
		checkNumericParser<double>();
		require(
			parseValue<int>("999999999999999999999999999") == std::numeric_limits<int>::max(),
			"Overflow must retain the existing stream saturation behavior");
		require(
			parseValue<unsigned int>("-1") == std::numeric_limits<unsigned int>::max(),
			"Unsigned negative input must retain stream behavior");
		require(parseValue<double>("-1.25e2") == -125.0, "Floating exponents must still parse");
		require(parseValue<MyGUI::IntPoint>("1 2") == MyGUI::IntPoint(1, 2), "Integer points must parse");
		require(parseValue<MyGUI::FloatPoint>("1.5 2.5") == MyGUI::FloatPoint(1.5f, 2.5f), "Float points must parse");
		require(parseValue<MyGUI::IntSize>("3 4") == MyGUI::IntSize(3, 4), "Integer sizes must parse");
		require(parseValue<MyGUI::FloatSize>("3.5 4.5") == MyGUI::FloatSize(3.5f, 4.5f), "Float sizes must parse");
		require(parseValue<MyGUI::IntRect>("1 2 3 4") == MyGUI::IntRect(1, 2, 3, 4), "Integer rectangles must parse");
		require(parseValue<MyGUI::FloatRect>("1 2 3 4") == MyGUI::FloatRect(1, 2, 3, 4), "Float rectangles must parse");
		require(
			parseValue<MyGUI::FloatCoord>("1 2 3 4") == MyGUI::FloatCoord(1, 2, 3, 4),
			"Float coordinates must parse");
		require(
			parseValue<MyGUI::DoubleCoord>("1 2 3 4") == MyGUI::DoubleCoord(1, 2, 3, 4),
			"Double coordinates must parse");
		require(
			parseValue<MyGUI::IntCoord>("1 2 3 4 \n") == MyGUI::IntCoord(1, 2, 3, 4),
			"Coordinates must allow trailing whitespace");
		for (auto input : {"1 2", "1 2 3 x", "1 2 3 4 5"})
			require(
				parseValue<MyGUI::IntCoord>(input) == MyGUI::IntCoord{},
				"Incomplete or trailing coordinates must reset");
		require(
			parseValue<bool>("True") && !parseValue<bool>(" true "),
			"Bool specialization must keep exact matching");
		require(
			parseValue<char>("65") == 'A' && parseValue<unsigned char>("66") == 'B',
			"Character parsing must remain numeric");
	}

	class CommaDecimal : public std::numpunct<char>
	{
	protected:
		char do_decimal_point() const override
		{
			return ',';
		}
	};

	void testFormattingAndLocale()
	{
		using MyGUI::utility::toString;
		require(
			toString<int>(-42) == "-42" && toString<unsigned int>(42) == "42",
			"Integer formatting must be preserved");
		require(
			toString<long>(-42) == "-42" && toString<unsigned long>(42) == "42",
			"Long formatting must be preserved");
		require(
			toString<long long>(-42) == "-42" && toString<unsigned long long>(42) == "42",
			"Wide integer formatting must be preserved");
		require(
			toString<float>(1.25f) == "1.25" && toString<double>(1.25) == "1.25",
			"Float formatting must be preserved");
		require(
			toString(true) == "true" && toString("value=", 42, '/', false) == "value=42/0",
			"Single and variadic formatting must keep their distinct bool behavior");
		struct RestoreLocale
		{
			std::locale previous = std::locale();
			~RestoreLocale()
			{
				std::locale::global(previous);
			}
		} restore;
		std::locale::global(std::locale(std::locale::classic(), new CommaDecimal));
		require(
			MyGUI::utility::parseValue<double>("1,25") == 1.25,
			"Shared parsing must use the current global locale");
		require(toString<double>(1.25) == "1,25", "Shared formatting must use the current global locale");
		require(
			MyGUI::utility::parseValue<MyGUI::FloatPoint>("1,5 2,5") == MyGUI::FloatPoint(1.5f, 2.5f),
			"Geometry parsing must preserve locale");
	}

	void testCompatibilityTables()
	{
		using Compatibility = MyGUI::BackwardCompatibility;
		MyGUI::LogManager logs;
		MyGUI::LayoutManager layouts;
		for (int cycle = 0; cycle < 2; ++cycle)
		{
			Compatibility::initialise();
			Compatibility::initialise();
#ifndef MYGUI_DONT_USE_OBSOLETE
			require(
				Compatibility::getPropertyRename("ButtonPressed") == "StateSelected" &&
					Compatibility::getPropertyRename("AlignVert") == "VerticalAlignment",
				"Legacy property aliases must survive initialization");
			require(
				Compatibility::getSkinRename("StaticImage") == "ImageBox" &&
					Compatibility::getSkinRename("ButtonMinusPlus") == "ButtonExpandSkin",
				"Legacy skin aliases must survive initialization");
			require(
				Compatibility::isIgnoreProperty("SkinLine") && Compatibility::isIgnoreProperty("HeightLine") &&
					Compatibility::isIgnoreProperty("ButtonSkin"),
				"Deduplicated ignore entries must remain present");
#else
			require(
				Compatibility::getPropertyRename("ButtonPressed") == "ButtonPressed" &&
					Compatibility::getSkinRename("StaticImage") == "StaticImage" &&
					!Compatibility::isIgnoreProperty("SkinLine"),
				"Obsolete-disabled builds must leave legacy names unchanged");
#endif
			require(
				Compatibility::getPropertyRename("Widget_Custom") == "Custom" &&
					Compatibility::getPropertyRename("Widget_Custom", false) == "Widget_Custom",
				"Prefix stripping must preserve its opt-out");
			require(
				Compatibility::getSkinRename("UnknownSkin") == "UnknownSkin" &&
					!Compatibility::isIgnoreProperty("UnknownProperty"),
				"Unknown names must remain unchanged");
			Compatibility::shutdown();
			require(
				Compatibility::getPropertyRename("ButtonPressed") == "ButtonPressed",
				"Shutdown must clear the property alias table");
		}
	}

}

int main()
{
	return unittest::runTests({
		{"Successful, nonthrowing and throwing casts", testCasts},
		{"Any storage, assignment and exception safety", testAnyStorage},
		{"Any assignment from nested values", testAnyNestedAssignment},
		{"Any pointer stability across moves", testAnyPointerStability},
		{"Any comparison and unsupported types", testAnyComparison},
		{"Const and mutable user-data access", testUserDataConstAccess},
		{"Shared numeric and geometry parsers", testParsing},
		{"Formatting and global locale", testFormattingAndLocale},
		{"Compatibility table lifecycle", testCompatibilityTables},
	});
}
