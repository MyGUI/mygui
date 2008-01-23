/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_InputManager.h"
#include <Ogre.h> // FIXME

namespace MyGUI
{

	const size_t TEXTURE_SIZE = 512;

	RenderBox::RenderBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mUserViewport(0),
		mEntity(null),
		mRotationSpeed(0),
		mBackgroungColour(Ogre::ColourValue::Blue),
		mMouseRotation(0)
	{
		MYGUI_DEBUG_ASSERT(mSubSkinChild.size() == 1, "subskin must be one");
		MYGUI_DEBUG_ASSERT(false == mSubSkinChild[0]->_isText(), "subskin must be not text");
		mElementSkin = mSubSkinChild.front();

		createRenderMaterial();

		mPointer = "hand"; // дл€ теста
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
	void RenderBox::injectObject(const Ogre::String& _meshName)
	{
		clear();

		mEntity = mScene->createEntity(util::toString(this, "_RenderBoxMesh_", _meshName), _meshName);
		mNode->attachObject(mEntity);

		updateViewport();
	}

	// очищает сцену
	void RenderBox::clear()
	{
		setAutorotationSpeed(0);
		setRotationAngle(Ogre::Degree(0));

		if (mEntity) {
			mNode->detachObject(mEntity);
			mScene->destroyEntity(mEntity);
			mEntity = 0;
		}
	}

	void RenderBox::setAutorotationSpeed(int _speed)
	{
		if (mRotationSpeed == _speed) return;
		mRotationSpeed = _speed;

		if (mRotationSpeed) {
			Gui::getInstance().addFrameListener(this);
		}
		else {
			Gui::getInstance().removeFrameListener(this);
		}
	}

	void RenderBox::setBackgroungColour(const Ogre::ColourValue & _colour)
	{
		if (0 == mUserViewport){
			mBackgroungColour = _colour;
			Ogre::Viewport *v = mTexture->getViewport(0);
			v->setBackgroundColour(mBackgroungColour);
		}
	}

	void RenderBox::setRotationAngle(const Ogre::Degree & _rotationAngle)
	{
		if (0 == mUserViewport){
			mNode->resetOrientation();
			mNode->yaw(Ogre::Radian(_rotationAngle));
		}
	}

	void RenderBox::setMouseRotation(bool _enable)
	{
		mMouseRotation = _enable;
	}

	void RenderBox::setRenderTarget(Ogre::Camera * _camera)
	{
		clear();
		Ogre::MaterialManager * manager = Ogre::MaterialManager::getSingletonPtr();
		if (manager != 0) manager->remove(mMaterial);

		mUserViewport = 1;
		mRttCam = _camera;

		mMaterial = util::toString(this, "_MaterialRenderBox");
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(mMaterial, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState(util::toString(this, "_TextureRenderBox"));

		// Setup Render To Texture for preview window
		Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture(util::toString(this, "_TextureRenderBox"));
		manager->remove(util::toString(this, "_TextureRenderBox"));

		Ogre::TextureManager::getSingleton().remove(util::toString(this, "_TextureRenderBox"));
		mTexture = Ogre::TextureManager::getSingleton().createManual(util::toString(this, "_TextureRenderBox"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET)
			->getBuffer()->getRenderTarget();

		//mRttCam->setAspectRatio(getWidth()/getHeight());

		Ogre::Viewport *v = mTexture->addViewport( mRttCam );
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame( true );
		v->setBackgroundColour(mBackgroungColour);
		v->setShadowsEnabled(true);
		v->setSkiesEnabled(false);

		mElementSkin->_setMaterialName(mMaterial);
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

	void RenderBox::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		if (mMouseRotation){
			mNode->yaw(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			mLastPointerX = _left;
		}
	}

	void RenderBox::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (mMouseRotation){
			const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
			mLastPointerX = point.left;
		}
	}

	void RenderBox::_onMouseDrag(int _left, int _top)
	{
		notifyMouseDrag(this, _left, _top);

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseDrag(_left, _top);
	}

	void RenderBox::_onMouseButtonPressed(bool _left)
	{
		notifyMousePressed(this, _left);

		// !!! ќЅя«ј“≈Ћ№Ќќ вызывать в конце метода
		Widget::_onMouseButtonPressed(_left);
	}

	void RenderBox::createRenderMaterial()
	{
		// создаем новый сцен менеджер
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, util::toString(this, "_SceneManagerRenderBox"));

		// создаем нод к которуму будем вс€кую др€нь атачить
		mNode = mScene->getRootSceneNode()->createChildSceneNode();

		mScene->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		// главный источник света
		Ogre::Vector3 dir(-1, -1, 0.5);
		dir.normalise();
		Ogre::Light * light = mScene->createLight(util::toString(this, "_LightRenderBox"));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		// создаем материал
		mMaterial = util::toString(this, "_MaterialRenderBox");
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(mMaterial, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState(util::toString(this, "_TextureRenderBox"));

		mTexture = Ogre::TextureManager::getSingleton().createManual(util::toString(this, "_TextureRenderBox"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET)
			->getBuffer()->getRenderTarget();

		mRttCam = mScene->createCamera(util::toString(this, "_CameraRenderBox"));
		mCamNode = mScene->getRootSceneNode()->createChildSceneNode(util::toString(this, "_CameraNodeRenderBox"));
		mCamNode->attachObject(mRttCam);
		mRttCam->setNearClipDistance(1);
		mRttCam->setAspectRatio(getWidth()/getHeight());

		Ogre::Viewport *v = mTexture->addViewport( mRttCam );
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame( true );
		v->setBackgroundColour(mBackgroungColour);
		v->setShadowsEnabled(true);
		v->setSkiesEnabled(false);

		mElementSkin->_setMaterialName(mMaterial);
	}

	void RenderBox::updateViewport()
	{
		// при нуле вылетает
		if ((getWidth() <= 1) || (getHeight() <= 1)) return;

		if (0 == mUserViewport){
			mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());

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
		}
	}

} // namespace MyGUI
