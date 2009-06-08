
# define additional include directories
include_directories(.)

# define the sources
set( sources
	MyGUI_FlowContainer.h
	MyGUI_SizeDescription.h
	MyGUI_Spacer.cpp
	MyGUI_Factories_temp.h
	MyGUI_FlowContainer.cpp
	CMakeLists.txt
	MyGUI_Container.h
	MyGUI_Spacer.h
	MyGUI_SizeDescription.cpp
	DemoKeeper.h
	MyGUI_Container.cpp
	DemoKeeper.cpp
	ContainerFactory.h
)

# setup MyGUIEngine target
add_executable(test_containers ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(test_containers PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(test_containers MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(test_containers
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

