/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_RenderBox.h"
#include "MyGUI_Gui.h"
#include <Ogre.h> // FIXME

namespace MyGUI
{

	const size_t RENDER_BOX_AUTO_ROTATION_SPEED = 20;

	RenderBox::RenderBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mEntity(null),
		mRttCam(null),
		mCamNode(null),
		mRotationSpeed(0)
	{
		MYGUI_DEBUG_ASSERT(mSubSkinChild.size() == 1, "subskin must be one");
		MYGUI_DEBUG_ASSERT(false == mSubSkinChild[0]->_isText(), "subskin must be not text");

		mElement = static_cast<PanelAlphaOverlayElement *>(mSubSkinChild[0]->_getOverlayElement());
		MYGUI_DEBUG_ASSERT(null != mElement, "overlay element not found");

		createRenderMaterial();
	}

	RenderBox::~RenderBox()
	{
		clear();

		Ogre::MaterialManager * manager = Ogre::MaterialManager::getSingletonPtr();
		if (manager != 0) manager->remove(mMaterial);

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root) root->destroySceneManager(mScene);

	}

	// добавл€ет в сцену объект, старый удал€етьс€
	void RenderBox::injectObject(Ogre::String _name, Ogre::String _meshName)
	{
		clear();

		mEntity = mScene->createEntity(_name, _meshName);
		mNode->attachObject(mEntity);

		updateViewport();
	}

	// очищает сцену
	void RenderBox::clear()
	{
		setAutoRotate(0);

		if (mEntity) {
			mNode->detachObject(mEntity);
			mScene->destroyEntity(mEntity);
			mEntity = 0;
		}
	}

	void RenderBox::setAutoRotate(int _speed)
	{
		if (mRotationSpeed == _speed) return;
		mRotationSpeed = _speed;
		if (1 == _speed) mRotationSpeed = RENDER_BOX_AUTO_ROTATION_SPEED;

		if (mRotationSpeed) {
			Gui::getInstance().addFrameListener(this);
		}
		else {
			Gui::getInstance().removeFrameListener(this);
		}
	}

	void RenderBox::setPosition(const IntCoord& _coord)
	{
		if (null != mRttCam) updateViewport();
		Widget::setPosition(_coord);
	}

	void RenderBox::setSize(const IntSize& _size)
	{
		if (null != mRttCam) updateViewport();
		Widget::setSize(_size);
	}

	void RenderBox::_frameEntered(float _time)
	{
		if (mRotationSpeed) mNode->yaw(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
	}

	void RenderBox::createRenderMaterial()
	{
		// создаем новый сцен менеджер
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "SceneManagerRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));

		// создаем нод к которуму будем вс€кую др€нь атачить
		mNode = mScene->getRootSceneNode()->createChildSceneNode();

		mScene->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		// главный источник света
		Ogre::Vector3 dir(-1, -1, 0.5);
		dir.normalise();
		Ogre::Light * light = mScene->createLight("LightDirectRender_" + Ogre::StringConverter::toString((Ogre::uint32)this));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		// создаем материал
		mMaterial = "MaterialRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this);
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(mMaterial, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState("TextureRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));

		// Setup Render To Texture for preview window
		mTexture = Ogre::Root::getSingleton().getRenderSystem()->createRenderTexture( "TextureRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this), 512, 512, Ogre::TEX_TYPE_2D, Ogre::PF_R8G8B8 );

		mRttCam = mScene->createCamera("CameraRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));
		mCamNode = mScene->getRootSceneNode()->createChildSceneNode("CameraNodeRenderBox_" + Ogre::StringConverter::toString((Ogre::uint32)this));
		mCamNode->attachObject(mRttCam);
		mRttCam->setNearClipDistance(1);
		mRttCam->setAspectRatio(getWidth()/getHeight());

		Ogre::Viewport *v = mTexture->addViewport( mRttCam );
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame( true );
		v->setBackgroundColour( Ogre::ColourValue::Blue);
		v->setShadowsEnabled(true);
		v->setSkiesEnabled(false);

		mElement->setMaterialName(mMaterial);
	}

	void RenderBox::updateViewport()
	{

		if ((float)getWidth()/(float)getHeight() > 0)mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());

		// вычисл€ем рассто€ние, чтобы был виден весь объект
		const Ogre::AxisAlignedBox & box = mEntity->getBoundingBox();
		box.getCenter();
		Ogre::Vector3 vec = box.getSize();

		float width = sqrt(vec.x*vec.x + vec.z*vec.z); // самое длинное - диагональ (если крутить модель)
		float len2 = width / mRttCam->getAspectRatio();
		float height = vec.y;
		float len1 = height;
		if (len1 < len2) len1 = len2;

		len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view

		// центр объекта + отъехать так, чтобы влезла ближн€€ грань BoundingBox'а + чуть вверх и еще назад дл€ красоты
		mCamNode->setPosition(box.getCenter() + Ogre::Vector3(0, 0, vec.z/2 + len1) + Ogre::Vector3(0, height*0.1, len1*0.2));
		mCamNode->lookAt(box.getCenter(), Ogre::Node::TS_WORLD);
		mCamNode->showBoundingBox(true);
	}

} // namespace MyGUI