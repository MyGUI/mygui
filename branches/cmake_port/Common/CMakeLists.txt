
include_directories(
 .
 ${MYGUI_SOURCE_DIR}/Common
 ${MYGUI_SOURCE_DIR}/Common/Base
 ${MYGUI_SOURCE_DIR}/MyGUIEngine/include
 ${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/include
)

set(commonfiles
	ItemBox/BaseItemBox.h
	ItemBox/ItemDropInfo.h
	ItemBox/BaseCellView.h
	precompiled.cpp
	RenderBox/RenderBoxWrap.cpp
	RenderBox/RenderBoxWrap.h
	PanelView/BasePanelViewItem.h
	PanelView/BasePanelView.h
	PanelView/BasePanelViewCell.h
	CMakeLists.txt
	Base/BaseManager.h
	Base/BaseResource.rc
	Base/Ogre/BaseManager.h
	Base/Ogre/BaseManager.cpp
	#Base/DirectX
	#Base/DirectX/BaseManager.h
	#Base/DirectX/BaseManager.cpp
	Base/BaseManager.cpp
	Base/StatisticInfo.h
	Base/mygui.ico
	precompiled.h
	BaseLayout/BaseLayout.h
	${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreRenderManager.cpp
	${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreTexture.cpp
	${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreVertexBuffer.cpp
	${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreDataManager.cpp
	${MYGUI_SOURCE_DIR}/RenderSystems/Ogre/OgreRenderSystem/src/MyGUI_OgreViewport.cpp
)

add_definitions("-DBASE_PLATFORM_OGRE")

# setup MyGUIEngine target
add_library(MyGUIFramework ${commonfiles})

add_dependencies(MyGUIFramework MyGUIEngine)

