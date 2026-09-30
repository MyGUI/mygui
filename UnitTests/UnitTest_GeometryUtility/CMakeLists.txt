mygui_unit_test(UnitTest_GeometryUtility)
# GeometryUtility is internal and not exported by shared engine builds.
target_sources(UnitTest_GeometryUtility PRIVATE "${MYGUI_SOURCE_DIR}/MyGUIEngine/src/MyGUI_GeometryUtility.cpp")
