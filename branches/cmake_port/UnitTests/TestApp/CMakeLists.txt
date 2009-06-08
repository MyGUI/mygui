
# define additional include directories
include_directories(.)

# define the sources
set( sources
	AbilityObject.h
	IBase.h
	BaseManager.h
	ParticleState.h
	RandomController.h
	FadeController.h
	MyGUI_RTTLayer.cpp
	IAnimationLink.h
	RobotObject.h
	MyGUI_LayerNodeAnimation.h
	AnimationFactory.h
	SoundState.h
	IAnimationGraph.h
	KinematicalObject.h
	AnimationGraph.h
	IAnimationState.h
	MyGUI_GenericFactory.h
	MyGUI_RTTLayer.h
	WobbleNodeAnimator.cpp
	WobbleNodeAnimator.h
	IAnimationController.h
	DemoKeeper.h
	MyGUI_RTTLayerNode.h
	IKinematicalObject.h
	SkeletonState.h
	BaseManager.cpp
	AnimationFactory.cpp
	DemoKeeper.cpp
	IRenderableObject.h
	TimeContext.h
	DelegateImplement.h
	RenderableObject.h
	LoopController.h
	MyGUI_RTTLayerFactory.h
	Delegate.h
	AnimatebleObject.h
	MyGUI_RTTLayerNode.cpp
)


# setup MyGUIEngine target
add_executable(testapp ${sources})

# set some platform specific things
if(WIN32)
  set_target_properties(testapp PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif(WIN32)

# add dependencies
add_dependencies(testapp MyGUIEngine MyGUIFramework MyGUIOgreRenderSystem)

# link libraries against it
target_link_libraries(testapp
 ${OGRE_LIBRARIES}
 ${OIS_LIBRARIES}
 MyGUIEngine
 MyGUIFramework
 MyGUIOgreRenderSystem
 uuid
)

