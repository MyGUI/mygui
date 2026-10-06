# Exercise static-library visibility even when the main engine is built shared.
# Use real engine types without pulling in rendering or resource dependencies.
set(MYGUI_STATIC ON)
configure_file(
	"${MYGUI_SOURCE_DIR}/MyGUIEngine/include/MyGUI_Config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/MyGUI_Config.h"
	@ONLY
)

add_library(MyGUIStaticRttiArchive STATIC
	"${MYGUI_SOURCE_DIR}/MyGUIEngine/src/MyGUI_IObject.cpp"
	"${MYGUI_SOURCE_DIR}/MyGUIEngine/src/MyGUI_ISerializable.cpp"
)
add_library(MyGUIStaticRttiBridge SHARED StaticRttiBridge.cpp)
target_link_libraries(MyGUIStaticRttiBridge PRIVATE MyGUIStaticRttiArchive)

add_executable(UnitTest_StaticRtti StaticRttiTest.cpp)
# Do not link the archive here: RTTI must be provided by the containing shared library.
target_link_libraries(UnitTest_StaticRtti PRIVATE MyGUIStaticRttiBridge)

foreach(_target MyGUIStaticRttiArchive MyGUIStaticRttiBridge UnitTest_StaticRtti)
	target_include_directories(${_target} PRIVATE
		"${MYGUI_SOURCE_DIR}/MyGUIEngine/include"
		"${CMAKE_CURRENT_BINARY_DIR}/include"
	)
	target_compile_features(${_target} PRIVATE cxx_std_17)
	set_target_properties(${_target} PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN YES
		POSITION_INDEPENDENT_CODE ON
		FOLDER UnitTest
	)
endforeach()

add_test(NAME UnitTest_StaticRtti COMMAND UnitTest_StaticRtti)
