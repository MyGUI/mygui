/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_InputManager.h"

#include <OgreTextureManager.h>

namespace MyGUI
{

	const size_t TEXTURE_SIZE = 512;

	RenderBox::RenderBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mUserViewport(false),
		mEntity(null),
		mBackgroungColour(Ogre::ColourValue::Blue),
		mMouseRotation(false),
		mLeftPressed(false),
		mRotationSpeed(RENDER_BOX_AUTO_ROTATION_SPEED),
		mAutoRotation(false)
	{

		// �������������� �������������
		MYGUI_DEBUG_ASSERT(null != mMainSkin, "need one subskin");

		// ��������� ������������ ������
		mPointerKeeper = mPointer;
		mPointer.clear();

		createRenderTexture();
	}

	RenderBox::~RenderBox()
	{
		clear();

		if (mRotationSpeed) Gui::getInstance().removeFrameListener(this);

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);
	}

	// ��������� � ����� ������, ������ ����������
	void RenderBox::injectObject(const Ogre::String& _meshName)
	{
		if(mUserViewport) {
			mUserViewport = false;
			createRenderTexture();
		}

		clear();

		mEntity = mScene->createEntity(utility::toString(this, "_RenderBoxMesh_", _meshName), _meshName);
		mNode->attachObject(mEntity);
		mPointer = mMouseRotation ? mPointerKeeper : "";

		updateViewport();
	}

	// ������� �����
	void RenderBox::clear()
	{
		setRotationAngle(Ogre::Degree(0));

		if (mEntity) {
			mNode->detachObject(mEntity);
			mScene->destroyEntity(mEntity);
			mEntity = 0;
		}
	}

	void RenderBox::setAutoRotationSpeed(int _speed)
	{
		mRotationSpeed = _speed;
	}

	void RenderBox::setBackgroungColour(const Ogre::ColourValue & _colour)
	{
		if (false == mUserViewport){
			mBackgroungColour = _colour;
			Ogre::Viewport *v = mTexture->getViewport(0);
			v->setBackgroundColour(mBackgroungColour);
		}
	}

	void RenderBox::setRotationAngle(const Ogre::Degree & _rotationAngle)
	{
		if (false == mUserViewport){
			mNode->resetOrientation();
			mNode->yaw(Ogre::Radian(_rotationAngle));
		}
	}

	void RenderBox::setMouseRotation(bool _enable)
	{
		mMouseRotation = _enable;
		mPointer = (mMouseRotation && mEntity) ? mPointerKeeper : "";
	}

	void RenderBox::setRenderTarget(Ogre::Camera * _camera)
	{
		// ������ �������
		clear();
		mPointer = "";

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);
		mScene = 0;

		// ������� ����� ��������
		mUserViewport = true;
		mRttCam = _camera;

		std::string texture(utility::toString(this, "_TextureRenderBox"));

		Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture(texture);

		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		manager.remove(texture);

		mTexture = manager.createManual(texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET)
			->getBuffer()->getRenderTarget();

		Ogre::Viewport *v = mTexture->addViewport( mRttCam );
		v->setClearEveryFrame(true);

		_setTextureName(texture);
	}

	void RenderBox::setPosition(const IntCoord& _coord)
	{
		updateViewport();
		Widget::setPosition(_coord);
	}

	void RenderBox::setSize(const IntSize& _size)
	{
		updateViewport();
		Widget::setSize(_size);
	}

	void RenderBox::_frameEntered(float _time)
	{
		if ((false == mUserViewport) && (mAutoRotation) && (false == mLeftPressed)) mNode->yaw(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
	}

	void RenderBox::_onMouseDrag(int _left, int _top)
	{
		if ((false == mUserViewport) && mMouseRotation && mAutoRotation) {
			mNode->yaw(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			mLastPointerX = _left;
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseDrag(_left, _top);
	}

	void RenderBox::_onMouseButtonPressed(bool _left)
	{
		if (mMouseRotation && mAutoRotation) {
			const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
			mLastPointerX = point.left;
			mLeftPressed = true;
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseButtonPressed(_left);
	}

	void RenderBox::_onMouseButtonReleased(bool _left)
	{
		if (_left) mLeftPressed = false;

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseButtonReleased(_left);
	}

	void RenderBox::createRenderTexture()
	{
		mPointer = mMouseRotation ? mPointerKeeper : "";

		// ������� ����� ���� ��������
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, utility::toString(this, "_SceneManagerRenderBox"));

		// ������� ��� � �������� ����� ������ ����� �������
		mNode = mScene->getRootSceneNode()->createChildSceneNode();

		mScene->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		// ������� �������� �����
		Ogre::Vector3 dir(-1, -1, 0.5);
		dir.normalise();
		Ogre::Light * light = mScene->createLight(utility::toString(this, "_LightRenderBox"));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		std::string texture(utility::toString(this, "_TextureRenderBox"));
		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		manager.remove(texture);
		mTexture = manager.createManual(texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET)
			->getBuffer()->getRenderTarget();

		std::string camera(utility::toString(this, "_CameraRenderBox"));
		mRttCam = mScene->createCamera(camera);
		mCamNode = mScene->getRootSceneNode()->createChildSceneNode(camera);
		mCamNode->attachObject(mRttCam);
		mRttCam->setNearClipDistance(1);
		if (getHeight() == 0) mRttCam->setAspectRatio(1);
		else mRttCam->setAspectRatio(getWidth()/getHeight());

		Ogre::Viewport *v = mTexture->addViewport( mRttCam );
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame( true );
		v->setBackgroundColour(mBackgroungColour);
		v->setShadowsEnabled(true);
		v->setSkiesEnabled(false);

		_setTextureName(texture);
	}

	void RenderBox::updateViewport()
	{
		// ��� ���� ��������
		if ((getWidth() <= 1) || (getHeight() <= 1) ) return;

		if ((false == mUserViewport) && (null != mEntity) && (null != mRttCam)) {
			// �� ����, ����� �� ����������� ������, ������������� ������
			mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());

			// ��������� ����������, ����� ��� ����� ���� ������
			const Ogre::AxisAlignedBox & box = mEntity->getBoundingBox();
			box.getCenter();
			Ogre::Vector3 vec = box.getSize();

			float width = sqrt(vec.x*vec.x + vec.z*vec.z); // ����� ������� - ��������� (���� ������� ������)
			float len2 = width / mRttCam->getAspectRatio();
			float height = vec.y;
			float len1 = height;
			if (len1 < len2) len1 = len2;

			len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view

			// ����� ������� �� ��������� + �������� ���, ����� ������ ������� ����� BoundingBox'� + ���� ����� � ��� ����� ��� �������
			mCamNode->setPosition(box.getCenter() + Ogre::Vector3(0, 0, vec.z/2 + len1) + Ogre::Vector3(0, height*0.1, len1*0.2));
			mCamNode->lookAt(Ogre::Vector3(0, box.getCenter().y, 0), Ogre::Node::TS_WORLD);
		}
	}

	void RenderBox::setAutoRotation(bool _auto)
	{
		mAutoRotation = _auto;
		if (mAutoRotation) Gui::getInstance().addFrameListener(this);
		else Gui::getInstance().removeFrameListener(this);
	}

} // namespace MyGUI
