if(MYGUI_BUILD_RENDERSYSTEM_D3D9)
  add_subdirectory(DirectX/DirectXRenderSystem)
endif(MYGUI_BUILD_RENDERSYSTEM_D3D9)

if(MYGUI_BUILD_RENDERSYSTEM_OGRE)
  add_subdirectory(Ogre/OgreRenderSystem)
endif(MYGUI_BUILD_RENDERSYSTEM_OGRE)
