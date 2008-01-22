#include "stdafx.h"
#include <Ogre.h>
#include "MyGUI_GUI.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_AssetManager.h"
#include "MyGUI_WidgetFactory.h"

using namespace Ogre;
using namespace std;

namespace MyGUI
{

	class GUI;

	RenderBox::RenderBox(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, "", uOverlay, pWindowParent),
		m_entity(0),
		mRttCam(0),
		mCamNode(0),
		mFrameListener(false)
	{
		createRenderMaterial();
	}

	RenderBox::~RenderBox()
	{
		clear();

		Ogre::MaterialManager * manager = Ogre::MaterialManager::getSingletonPtr();
		if (manager != 0) manager->remove(m_material);

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root) root->destroySceneManager(m_scene);

	}

	// добавляет в сцену объект, старый удаляеться
	void RenderBox::injectObject(Ogre::String _name, Ogre::String _file)
	{
		clear(false);

		m_entity = m_scene->createEntity(_name, _file);
		m_node->attachObject(m_entity);

		// вычисляем расстояние, чтобы был виден весь объект
		/*const Ogre::AxisAlignedBox & box = m_entity->getBoundingBox();
		Ogre::Vector3 vec = box.getSize();

		float height2 = (vec.x > vec.z) ? height2 = vec.x : height2 = vec.z; 
		float len2 = height2*2.0*mRttCam->getAspectRatio();
		float height1 = vec.y;
		float len1 = height1*2.0 * mRttCam->getAspectRatio();
		if (len1 < len2) len1 = len2;

		mCamNode->setPosition(0, height1*0.5+height1*0.1, len1);
		mCamNode->lookAt(Vector3(0, height1*0.5, 0), Node::TS_WORLD);//*/


		// вычисляем расстояние, чтобы был виден весь объект
		const Ogre::AxisAlignedBox & box = m_entity->getBoundingBox();
		box.getCenter();
		Ogre::Vector3 vec = box.getSize();

		float height2 = sqrt(vec.x*vec.x + vec.z*vec.z); // самое длинное - диагональ (если крутить модель)
		float len2 = height2 / mRttCam->getAspectRatio();
		float height1 = vec.y;
		float len1 = height1;
		if (len1 < len2) len1 = len2;

		len1 /= 0.86;

		mCamNode->setPosition(box.getCenter().x, box.getCenter().y, box.getCenter().z + len1);
		mCamNode->lookAt(box.getCenter(), Node::TS_WORLD);
		mCamNode->showBoundingBox(true);//*/

		// подписываемся
		if (false == mFrameListener) {
			GUI::getSingleton()->addFrameListener(this);
			mFrameListener = true;
		}
	}

	// очищает сцену
	void RenderBox::clear(bool _frameClear)
	{
		// отписываемся
		if (mFrameListener && _frameClear) {
			GUI::getSingleton()->removeFrameListener(this);
			mFrameListener = false;
		}

		if (m_entity) {
			m_node->detachObject(m_entity);
			m_scene->destroyEntity(m_entity);
			m_entity = 0;
		}
	}

	void RenderBox::createRenderMaterial()
	{
		// создаем новый сцен менеджер
		m_scene = Ogre::Root::getSingleton().createSceneManager(ST_GENERIC, "SceneManagerRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));

		// создаем нод к которуму будем всякую дрянь атачить
		m_node = m_scene->getRootSceneNode()->createChildSceneNode();

		m_scene->setAmbientLight(ColourValue(0.8, 0.8, 0.8));

		// главный источник света
		Ogre::Vector3 dir(-1, -1, 0.5);
		dir.normalise();
		Ogre::Light * light = m_scene->createLight("LightDirectRender_" + Ogre::StringConverter::toString((Ogre::uint32)this));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		// создаем материал
		m_material = "MaterialRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this);
		MaterialPtr mat = MaterialManager::getSingleton().create(m_material, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("TextureRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));

        // Setup Render To Texture for preview window
		m_texture = Ogre::Root::getSingleton().getRenderSystem()->createRenderTexture( "TextureRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this), 512, 512, TEX_TYPE_2D, PF_R8G8B8 );
        {
            mRttCam = m_scene->createCamera("CameraRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));
            mCamNode = m_scene->getRootSceneNode()->createChildSceneNode("CameraNodeRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));
            mCamNode->attachObject(mRttCam);
			mRttCam->setNearClipDistance(1);
			//mRttCam->setAspectRatio(0.7);
            mCamNode->setPosition(0, 20, 50);
			mCamNode->lookAt(Vector3::ZERO, Node::TS_WORLD);

            Viewport *v = m_texture->addViewport( mRttCam );
            v->setOverlaysEnabled(false);
            v->setClearEveryFrame( true );
			v->setBackgroundColour( ColourValue::Blue);
			v->setShadowsEnabled(true);
			v->setSkiesEnabled(false);
        }

		// и устанавливаем материал
		m_overlayContainer->setMaterialName(m_material);
		
	}

	RenderBox *RenderBox::createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY, Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
	{
		
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		RenderBox * pWindow = new RenderBox(pSkin->subSkins[0], pSkin->SkinElement, parent ? OVERLAY_CHILD : uOverlay, parent ? parent->m_pWindowClient : NULL);
		
		pWindow->m_uAlign |= uAlign;
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX, SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);

		return pWindow;
	}

	bool RenderBox::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (m_node) m_node->yaw(Radian(evt.timeSinceLastFrame * 0.2));
		return true;
	}

	bool RenderBox::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

	void RenderBox::size(int16 iSizeX, int16 iSizeY) // изменяем размер окна
	{
		Window::size(iSizeX, iSizeY);
		if (mRttCam != 0) mRttCam->setAspectRatio((float)iSizeX / (float)iSizeY);
	}

	// регестрируем фабрику
	REGISTRY_WIDGET_FACTORY(WIDGET_RENDER_BOX, RenderBox);

} // namespace MyGUI