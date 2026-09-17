mygui_unit_test(UnitTest_RotatingPolygonalSkin)
target_link_libraries(UnitTest_RotatingPolygonalSkin PRIVATE MyGUISkinTestSupport)
# GeometryUtility is internal and not exported by shared engine builds.
target_sources(UnitTest_RotatingPolygonalSkin PRIVATE "${MYGUI_SOURCE_DIR}/MyGUIEngine/src/MyGUI_GeometryUtility.cpp")
