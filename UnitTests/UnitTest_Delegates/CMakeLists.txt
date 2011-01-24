if (Boost_FOUND)
	add_definitions("-DBOOST_FOUND")
endif ()

mygui_unit_test(UnitTest_Delegates)

if (Boost_FOUND)
	add_definitions("-DBOOST_FOUND")
endif ()