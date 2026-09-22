#include "PlatformFixture.h"
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	using namespace platformtest;
	std::string group, filter;
	std::filesystem::path artifacts = "platform-artifacts";
	bool visible = false, list = false;
	try
	{
		for (int i = 1; i < argc; ++i)
		{
			const std::string option = argv[i];
			if (option == "--visible")
				visible = true;
			else if (option == "--list")
				list = true;
			else if ((option == "--group" || option == "--case" || option == "--artifacts") && i + 1 < argc)
			{
				const std::string value = argv[++i];
				if (option == "--group")
					group = value;
				else if (option == "--case")
					filter = value;
				else
					artifacts = value;
			}
			else
				throw std::runtime_error("Unknown/incomplete option: " + option);
		}
		if (!group.empty() && group != "resources" && group != "rendering" && group != "lifecycle")
			throw std::runtime_error("Unknown test group: " + group);
		int passed = 0, failed = 0, skipped = 0, selected = 0;
		std::unique_ptr<Fixture> fixture;
		std::string currentGroup;
		for (const auto& test : cases())
		{
			if (!group.empty() && group != test.group)
				continue;
			if (!filter.empty() && test.name.find(filter) == std::string::npos)
				continue;
			++selected;
			if (list)
			{
				std::cout << test.group << '/' << test.name << '\n';
				continue;
			}
			if (!fixture || currentGroup != test.group || test.group == "lifecycle")
			{
				if (fixture)
					fixture->close();
				fixture.reset();
				fixture = std::make_unique<Fixture>(visible);
				fixture->open();
				currentGroup = test.group;
				fixture->caseName = "context-initialization";
				try
				{
					fixture->capture();
					fixture->expectCorners({0, 0, 0, 255});
					++passed;
					std::cout << "PASS " << test.group << "/context-initialization" << std::endl;
				}
				catch (const Fatal&)
				{
					throw;
				}
				catch (const std::exception& error)
				{
					++failed;
					std::cerr << "FAIL " << test.group << "/context-initialization: " << error.what() << std::endl;
					fixture->saveFailure(artifacts / (test.name + "-initialization.ppm"));
					fixture->resetCase();
				}
			}
			fixture->caseName = test.name;
			try
			{
				test.run(*fixture);
				++passed;
				std::cout << "PASS " << test.group << '/' << test.name << std::endl;
			}
			catch (const Fatal&)
			{
				fixture->saveFailure(artifacts / (test.name + ".ppm"));
				throw;
			}
			catch (const Skip& error)
			{
				++skipped;
				std::cout << "SKIP " << test.name << ": " << error.what() << std::endl;
			}
			catch (const std::exception& error)
			{
				++failed;
				std::cerr << "FAIL " << test.name << ": " << error.what() << std::endl;
				fixture->saveFailure(artifacts / (test.name + ".ppm"));
			}
			try
			{
				fixture->resetCase();
			}
			catch (const std::exception& error)
			{
				throw Fatal("Cleanup after " + test.name + ": " + error.what());
			}
		}
		require(selected != 0, "No cases matched the requested filter");
		if (fixture)
			fixture->close();
		if (!list)
			std::cout << "Backend " << platformtest::backendName() << ": " << passed << " passed, " << failed
					  << " failed, " << skipped << " skipped\n";
		return failed == 0 ? 0 : 1;
	}
	catch (const std::exception& error)
	{
		std::cerr << "FATAL backend " << platformtest::backendName() << ": " << error.what() << std::endl;
		return 1;
	}
}
