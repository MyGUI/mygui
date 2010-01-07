/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/

#include "RenderBoxScene.h"

namespace wraps
{

	RenderBoxScene::RenderBoxScene() :
		mCameraNode(nullptr),
		mCamera(nullptr),
		mEntity(nullptr),
		mAnimationState(nullptr),
		mRotationSpeed(RENDER_BOX_AUTO_ROTATION_SPEED),
		mMouseRotation(false),
		mLastPointerX(0),
		mLeftPressed(false),
		mAutoRotation(false),
		mFrameAdvise(false)
	{
	}

	RenderBoxScene::~RenderBoxScene()
	{
	}

	void RenderBoxScene::destroy()
	{
		clearScene();

		if (mCanvas)
		{
			frameAdvise(false);

			mCanvas->eventMouseDrag = nullptr;
			mCanvas->eventMouseButtonPressed = nullptr;
			mCanvas->eventMouseButtonReleased = nullptr;

			Ogre::Root* root = Ogre::Root::getSingletonPtr();
			if (root && mScene)
				root->destroySceneManager(mScene);
			mScene= nullptr;
		}

		RenderBox::destroy();
	}

	void RenderBoxScene::setCanvas(MyGUI::CanvasPtr _value)
	{
		RenderBox::setCanvas(_value);

		mCanvas->eventMouseDrag = newDelegate(this, &RenderBoxScene::notifyMouseDrag);
		mCanvas->eventMouseButtonPressed = newDelegate(this, &RenderBoxScene::notifyMouseButtonPressed);
		mCanvas->eventMouseButtonReleased = newDelegate(this, &RenderBoxScene::notifyMouseButtonReleased);

		createScene();
	}

	// ��������� � ����� ������, ������ ����������
	void RenderBoxScene::injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position, const Ogre::Quaternion & _orientation, const Ogre::Vector3 & _scale)
	{
		clearScene();

		static size_t num = 0;
		mEntity = mScene->createEntity(MyGUI::utility::toString(this, "_RenderBoxMesh_", _meshName, num++), _meshName);
		Ogre::SceneNode * node = mNode->createChildSceneNode(_position, _orientation);
		node->attachObject(mEntity);

		updateViewport();
	}

	void RenderBoxScene::setAnimation(const Ogre::String& _animation)
	{
		if (mEntity == nullptr)
			return;

		try
		{
			mAnimationState = mEntity->getAnimationState(_animation);
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
			mAnimationState->setWeight(1);
		}
		catch (Ogre::ItemIdentityException&)
		{
		}

		frameAdvise(needFrameUpdate());
	}

	// ������� �����
	void RenderBoxScene::clearScene()
	{
		setRotationAngle(Ogre::Degree(0));

		if (mScene)
		{
			mScene->destroyAllEntities();
			mNode->removeAndDestroyAllChildren();
		}

		mEntity = nullptr;
		mAnimationState = nullptr;
	}

	void RenderBoxScene::setAutoRotationSpeed(int _value)
	{
		mRotationSpeed = _value;
	}

	void RenderBoxScene::setRotationAngle(const Ogre::Degree& _value)
	{
		if (mNode)
		{
			mNode->resetOrientation();
			mNode->yaw(Ogre::Radian(_value));
		}
	}

	Ogre::Degree RenderBoxScene::getRotationAngle()
	{
		return Ogre::Degree(mNode->getOrientation().getYaw());
	}

	void RenderBoxScene::setMouseRotation(bool _enable)
	{
		mMouseRotation = _enable;
	}

	void RenderBoxScene::frameEntered(float _time)
	{
		if (!mLeftPressed)
		{
			if (mAutoRotation)
				mNode->yaw(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
		}
		if (mAnimationState)
			mAnimationState->addTime(_time);
	}

	void RenderBoxScene::createScene()
	{
		// ������� ����� ���� ��������
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, MyGUI::utility::toString(this, "_SceneManagerRenderBox"));

		// ������� ��� � �������� ����� ������ ����� �������
		mNode = mScene->getRootSceneNode()->createChildSceneNode();

		mScene->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		// ������� �������� �����
		Ogre::Vector3 dir(-1, -1, 0.5);
		dir.normalise();
		Ogre::Light * light = mScene->createLight(MyGUI::utility::toString(this, "_LightRenderBox"));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		std::string camera(MyGUI::utility::toString(this, "_CameraRenderBox"));
		mCamera = mScene->createCamera(camera);
		mCamera->setNearClipDistance(1);

		mCameraNode = mScene->getRootSceneNode()->createChildSceneNode(camera);
		mCameraNode->attachObject(mCamera);

		if (mCanvas->getHeight() == 0)
			mCamera->setAspectRatio(1);
		else
			mCamera->setAspectRatio( float(mCanvas->getWidth()) / float(mCanvas->getHeight()) );

		setViewport(mCamera);
	}

	void RenderBoxScene::updateViewport()
	{
		// ��� ���� ��������
		if ((mCanvas->getWidth() <= 1) || (mCanvas->getHeight() <= 1))
			return;

		if ((nullptr != mEntity) && (nullptr != mCamera))
		{
			// �� ����, ����� �� ����������� ������, ������������� ������
			mCamera->setAspectRatio((float)mCanvas->getWidth() / (float)mCanvas->getHeight());

			// ��������� ����������, ����� ��� ����� ���� ������
			Ogre::AxisAlignedBox box;

			box.merge(mEntity->getBoundingBox().getMinimum() + mEntity->getParentSceneNode()->_getDerivedPosition());
			box.merge(mEntity->getBoundingBox().getMaximum() + mEntity->getParentSceneNode()->_getDerivedPosition());

			if (box.isNull()) return;

			Ogre::Vector3 vec = box.getSize();

			float width = sqrt(vec.x*vec.x + vec.z*vec.z); // ����� ������� - ��������� (���� ������� ������)
			float len2 = width / mCamera->getAspectRatio();
			float height = vec.y;
			float len1 = height;
			if (len1 < len2) len1 = len2;
			len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view
			// ����� ������� �� ��������� + �������� ���, ����� ������ ������� ����� BoundingBox'� + ���� ����� � ��� ����� ��� �������
			Ogre::Vector3 result = box.getCenter() + Ogre::Vector3(0, 0, vec.z/2 + len1) + Ogre::Vector3(0, height*0.1, len1*0.2);
			Ogre::Vector3 look = Ogre::Vector3(0, box.getCenter().y /*+ box.getCenter().y * (1-mCurrentScale)*/, 0);

			mCameraNode->setPosition(result);
			mCameraNode->lookAt(look, Ogre::Node::TS_WORLD);
		}
	}

	void RenderBoxScene::setAutoRotation(bool _auto)
	{
		mAutoRotation = _auto;
		frameAdvise(needFrameUpdate());
	}

	void RenderBoxScene::requestUpdateCanvas(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event)
	{
		RenderBox::requestUpdateCanvas(_canvas, _event);

		updateViewport();
	}

	void RenderBoxScene::frameAdvise(bool _advise)
	{
		if (_advise && !mFrameAdvise && needFrameUpdate())
		{
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &RenderBoxScene::frameEntered);
			mFrameAdvise = true;
		}
		else if (!_advise && mFrameAdvise && !needFrameUpdate())
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &RenderBoxScene::frameEntered);
			mFrameAdvise = false;
		}
	}

	void RenderBoxScene::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		if (mMouseRotation)
		{
			mNode->yaw(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			mLastPointerX = _left;
		}
	}

	void RenderBoxScene::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			if (mMouseRotation)
			{
				const MyGUI::IntPoint & point = MyGUI::InputManager::getInstance().getLastLeftPressed();
				mLastPointerX = point.left;
				mLeftPressed = true;
			}
		}
	}

	void RenderBoxScene::notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (MyGUI::MouseButton::Left == _id) mLeftPressed = false;
	}

} // namespace wraps
