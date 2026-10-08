#include "MyGUI_StringUtility.h"
#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_BackwardCompatibility.h"
#include "MyGUI_ILogListener.h"
#include <limits>
#include <locale>

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
		{"Shared numeric and geometry parsers", testParsing},
		{"Formatting and global locale", testFormattingAndLocale},
		{"Compatibility table lifecycle", testCompatibilityTables},
	});
}
