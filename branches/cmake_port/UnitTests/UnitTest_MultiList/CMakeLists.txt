
# define additional include directories
include_directories(.)

# define the sources
set( sources
	Mirror_MultiList.h
	UnitTest_MultiList.h
	Mirror_List.h
	BiIndexData.cpp
	DemoKeeper.h
	UnitTest_List.h
	DemoKeeper.cpp
	BiIndexData.h
)

# setup MyGUIEngine target
add_executable(test_multilists ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(test_multilists PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(test_multilists MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(test_multilists
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

