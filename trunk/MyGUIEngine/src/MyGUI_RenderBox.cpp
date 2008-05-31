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

	Ogre::String RenderBox::WidgetTypeName = "RenderBox";

	const size_t TEXTURE_SIZE = 512;

	RenderBox::RenderBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mUserViewport(false),
		mEntity(null),
		mBackgroungColour(Ogre::ColourValue::Blue),
		mMouseRotation(false),
		mLeftPressed(false),
		mRotationSpeed(RENDER_BOX_AUTO_ROTATION_SPEED),
		mAutoRotation(false),
		mEntityState(null),
		mScale(1.0f),
		mCurrentScale(1.0f),
		mUseScale(false)
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
		if (needFrameUpdate()) Gui::getInstance().removeFrameListener(this);

		clear();

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);
	}

	// ��������� � ����� ������, ������ ����������
	void RenderBox::injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position, const Ogre::Quaternion & _orientation, const Ogre::Vector3 & _scale)
	{
		if(mUserViewport) {
			mUserViewport = false;
			createRenderTexture();
		}

		static size_t num = 0;

		Ogre::Entity * entity = mScene->createEntity(utility::toString(this, "_RenderBoxMesh_", _meshName, num++), _meshName);
		Ogre::SceneNode * node = mNode->createChildSceneNode(_position, _orientation);
		node->attachObject(entity);
		mVectorEntity.push_back(entity);

		mPointer = mMouseRotation ? mPointerKeeper : "";

		if (mEntity == null) mEntity = entity;

		updateViewport();
	}

	// ������� �����
	void RenderBox::clear()
	{
		setRotationAngle(Ogre::Degree(0));

		//if (mEntity) {
			//Ogre::SkeletonManager::getSingleton().remove();
			//mNode->detachObject(mEntity);
		mScene->destroyAllEntities();
		mNode->removeAndDestroyAllChildren();
		mVectorEntity.clear();

		mEntity = 0;
		mEntityState = null;
		//}
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
		if (false == mUserViewport) {
			mNode->resetOrientation();
			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(_rotationAngle));
			#else
				mNode->yaw(Ogre::Radian(_rotationAngle));
			#endif
		}
	}

	Ogre::Degree RenderBox::getRotationAngle()
	{
		if (false == mUserViewport) {
			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z
				return Ogre::Degree(mNode->getOrientation().getRoll());
			#else
				return Ogre::Degree(mNode->getOrientation().getYaw());
			#endif
		}
		return Ogre::Degree(0);
	}

	void RenderBox::setMouseRotation(bool _enable)
	{
		mMouseRotation = _enable;
		mPointer = (mMouseRotation && mEntity) ? mPointerKeeper : "";
	}

	void RenderBox::setViewScale(bool _scale)
	{
		if (mUseScale == _scale) return;
		if (needFrameUpdate())
		{
			mUseScale = _scale;
			if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(this);
		}
		else
		{
			mUseScale = _scale;
			if (needFrameUpdate()) Gui::getInstance().addFrameListener(this);
		}
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
		if ((false == mUserViewport) && (mAutoRotation) && (false == mLeftPressed)) {
			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
			#else
				mNode->yaw(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
			#endif
		}
		if (null != mEntityState) mEntityState->addTime(_time);

		if (mCurrentScale != mScale) {

			if (mCurrentScale > mScale) {
				mCurrentScale -= _time * 0.7f;
				if (mCurrentScale < mScale) mCurrentScale = mScale;
			}
			else {
				mCurrentScale += _time * 0.7f;
				if (mCurrentScale > mScale) mCurrentScale = mScale;
			}

			updateViewport();
		}
	}

	void RenderBox::_onMouseDrag(int _left, int _top)
	{
		if ((false == mUserViewport) && mMouseRotation/* && mAutoRotation*/) {
			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			#else
				mNode->yaw(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			#endif
			mLastPointerX = _left;
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseDrag(_left, _top);
	}

	void RenderBox::_onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (mMouseRotation/* || mAutoRotation*/) {
			const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
			mLastPointerX = point.left;
			mLeftPressed = true;
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseButtonPressed(_left, _top, _id);
	}

	void RenderBox::_onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (MB_Left == _id) mLeftPressed = false;

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseButtonReleased(_left, _top, _id);
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
		// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
		#ifdef LEFT_HANDED_CS_UP_Z
			Ogre::Vector3 dir(10, 10, -10);
		#else
			Ogre::Vector3 dir(-1, -1, 0.5);
		#endif

		dir.normalise();
		Ogre::Light * light = mScene->createLight(utility::toString(this, "_LightRenderBox"));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		std::string texture(utility::toString(this, "_TextureRenderBox"));
		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		manager.remove(texture);
		mTexture = manager.createManual(texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_B8G8R8A8, Ogre::TU_RENDERTARGET)
			->getBuffer()->getRenderTarget();

		std::string camera(utility::toString(this, "_CameraRenderBox"));
		mRttCam = mScene->createCamera(camera);
		mRttCam->setNearClipDistance(0.00000001);

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
			Ogre::AxisAlignedBox box;// = mNode->_getWorldAABB();//mEntity->getBoundingBox();

			for (VectorEntity::iterator iter = mVectorEntity.begin(); iter!=mVectorEntity.end(); ++iter) {
				box.merge((*iter)->getBoundingBox().getMinimum() + (*iter)->getParentSceneNode()->_getDerivedPosition());
				box.merge((*iter)->getBoundingBox().getMaximum() + (*iter)->getParentSceneNode()->_getDerivedPosition());
			}

			if (box.isNull()) return;
			
			//box.getCenter();
			Ogre::Vector3 vec = box.getSize();

			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z

				float width = sqrt(vec.x*vec.x + vec.y*vec.y); // ����� ������� - ��������� (���� ������� ������)
				float len2 = width / mRttCam->getAspectRatio();
				float height = vec.z;
				float len1 = height;
				if (len1 < len2) len1 = len2;
				len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view
				// ����� ������� �� ��������� + �������� ���, ����� ������ ������� ����� BoundingBox'� + ���� ����� � ��� ����� ��� �������
				Ogre::Vector3 result = box.getCenter() - Ogre::Vector3(vec.y/2 + len1, 0, 0) - Ogre::Vector3(len1*0.2, 0, -height*0.1);
				result.x *= mCurrentScale;
				mCamNode->setPosition(result);

				Ogre::Vector3 x = Ogre::Vector3(0, 0, box.getCenter().z + box.getCenter().z * (1-mCurrentScale)) - mCamNode->getPosition();
				Ogre::Vector3 y = Ogre::Vector3(Ogre::Vector3::UNIT_Z).crossProduct(x);
				Ogre::Vector3 z = x.crossProduct(y);
				mCamNode->setOrientation(Ogre::Quaternion(
					x.normalisedCopy(),
					y.normalisedCopy(),
					z.normalisedCopy()));

			#else

				float width = sqrt(vec.x*vec.x + vec.z*vec.z); // ����� ������� - ��������� (���� ������� ������)
				float len2 = width / mRttCam->getAspectRatio();
				float height = vec.y;
				float len1 = height;
				if (len1 < len2) len1 = len2;
				len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view
				// ����� ������� �� ��������� + �������� ���, ����� ������ ������� ����� BoundingBox'� + ���� ����� � ��� ����� ��� �������
				Ogre::Vector3 result = box.getCenter() + Ogre::Vector3(0, 0, vec.z/2 + len1) + Ogre::Vector3(0, height*0.1, len1*0.2);
				result.z *= mCurrentScale;
				Ogre::Vector3 look = Ogre::Vector3(0, box.getCenter().y /*+ box.getCenter().y * (1-mCurrentScale)*/, 0);

				mCamNode->setPosition(result);
				mCamNode->lookAt(look, Ogre::Node::TS_WORLD);

			#endif
		}
	}

	void RenderBox::setAutoRotation(bool _auto)
	{
		if (mAutoRotation == _auto) return;
		if (needFrameUpdate())
		{
			mAutoRotation = _auto;
			if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(this);
		}
		else
		{
			mAutoRotation = _auto;
			if (needFrameUpdate()) Gui::getInstance().addFrameListener(this);
		}
	}

	void RenderBox::setAnimation(const Ogre::String& _animation)
	{
		if (null != mEntityState) {
			mEntityState = null;
			if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(this);
		}

		if (_animation.empty()) return;

		if (null == mEntity) return;
		Ogre::SkeletonInstance * skeleton = mEntity->getSkeleton();
		if (null == skeleton) return;
		Ogre::AnimationStateSet * anim_set = mEntity->getAllAnimationStates();
		// FIXME ������ ������ ����� ��� ��� ��� �� �������� ��� � ������������������ ����? � ��� �������, �� ���������� ������ ������
		// ��������� ��� getAnimationState - �� ��� ��� ��������� �� ����� � ������� �� �����������
		/*
		Ogre::AnimationState * state = anim_set->getAnimationState(_animation);
		if (state != null)
		{
			// ��� �� ��� ����� ������ ������ ���
		}
		*/
		Ogre::AnimationStateIterator iter = anim_set->getAnimationStateIterator();

		while (iter.hasMoreElements()) {
			Ogre::AnimationState * state = iter.getNext();
			if (_animation == state ->getAnimationName()) {

				// �������������
				Gui::getInstance().addFrameListener(this);

				mEntityState = state;
				mEntityState->setEnabled(true);
				mEntityState->setLoop(true);
				mEntityState->setWeight(1.0f);

				return;
			}
		}
		MYGUI_LOG(Warning, "Unable to to set animation '" << _animation << "' - current entity don't have such animation.");
	}

	void RenderBox::_onMouseWheel(int _rel)
	{
		if ( ! mUseScale) return;

		const float near_min = 0.5f;
		const float coef = 0.0005;

		mScale += (-_rel) * coef;

		if (mScale > 1) mScale = 1;
		else if (mScale < near_min) mScale = near_min;

	}

} // namespace MyGUI
