find_path(MYGUI_EGL_INCLUDE_DIR EGL/egl.h REQUIRED)
find_library(MYGUI_EGL_LIBRARY NAMES EGL REQUIRED)
mygui_unit_test(UnitTest_OpenGLESPlatform)
target_link_libraries(UnitTest_OpenGLESPlatform PRIVATE MyGUI.OpenGLESPlatform "${MYGUI_EGL_LIBRARY}")
target_include_directories(UnitTest_OpenGLESPlatform SYSTEM PRIVATE "${MYGUI_EGL_INCLUDE_DIR}")
set_tests_properties(UnitTest_OpenGLESPlatform PROPERTIES
	LABELS "platform;graphics" RUN_SERIAL TRUE TIMEOUT 120)
