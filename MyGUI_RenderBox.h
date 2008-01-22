#pragma once

#include "Ogre.h"
#include "MyGUI_Window.h"
#include "MyGUI_WidgetFactory.h"

namespace MyGUI
{

	class RenderBox : public Window, public Ogre::FrameListener
	{

		RenderBox(const __MYGUI_SUBSKIN_INFO * lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
		virtual ~RenderBox();

	public:
		static RenderBox *createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_DEFAULT);
	        
		// добавляет в сцену объект, старый удаляеться
		void injectObject(Ogre::String _name, Ogre::String _file);

		// очищает сцену
		void clear(bool _frameClear = true);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна


	private:
		void createRenderMaterial();

	private:
		Ogre::SceneManager * m_scene;
		Ogre::Entity * m_entity;
		Ogre::SceneNode * m_node;
		Ogre::String m_material;
		Ogre::RenderTexture * m_texture;

		Ogre::Camera* mRttCam;
		Ogre::SceneNode* mCamNode;

		bool mFrameListener;

	};

} // namespace MyGUI