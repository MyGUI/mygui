/*!
	@file
	@author		Evmenov Georgiy
	@author		Alexander Ptakhin
	@date		01/2008
	@module
*/
#include "MyGUI_TestRenderBox.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ISubWidget.h"

#include <OgreTextureManager.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	const float SYNC_TIMEOUT = 1 / 25.0f;
	const size_t TEXTURE_SIZE = 512;

	TestRenderBox::TestRenderBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Canvas(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mUserViewport(false),
		mEntity(nullptr),
		mRttCam(nullptr),
		mViewport(nullptr),
		mCamNode(nullptr),
		mRotationSpeed(RENDER_BOX_AUTO_ROTATION_SPEED),
		mBackgroungColour(Ogre::ColourValue::Blue),
		mMouseRotation(false),
		mLeftPressed(false),
		mAutoRotation(false),
		mEntityState(nullptr),
		mScale(1.0f),
		mCurrentScale(1.0f),
		mUseScale(false),
		mNodeForSync(nullptr),
		mSceneManagerForSync(nullptr)
	{
		initialiseWidgetSkin(_info);

		Canvas::requestUpdateCanvas = newDelegate( this, &TestRenderBox::updateCanvas );
	}

	TestRenderBox::~TestRenderBox()
	{
		//Gui::getInstance().removeFrameListener(newDelegate(this, &TestRenderBox::frameEntered));
		mTexture.setNull();
		clear();

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);

		shutdownWidgetSkin();
	}

	void TestRenderBox::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void TestRenderBox::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// �������������� �������������
		MYGUI_DEBUG_ASSERT(nullptr != mMainSkin, "need one subskin");

		// ��������� ������������ ������
		mPointerKeeper = mPointer;
		mPointer.clear();

		mMainSkin->_setUVSet(FloatRect(0, 0, 1, 1));
		createRenderTexture();
	}

	void TestRenderBox::shutdownWidgetSkin()
	{
	}

	// ��������� � ����� ������, ������ ����������
	void TestRenderBox::injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position, const Ogre::Quaternion & _orientation, const Ogre::Vector3 & _scale)
	{
		if(mUserViewport) {
			mUserViewport = false;
			createRenderTexture();
		}

		static size_t num = 0;

		Ogre::Entity * entity = mScene->createEntity(utility::toString(this, "_TestRenderBoxMesh_", _meshName, num++), _meshName);
		Ogre::SceneNode * node = mNode->createChildSceneNode(_position, _orientation);
		node->attachObject(entity);
		mVectorEntity.push_back(entity);

		mPointer = mMouseRotation ? mPointerKeeper : "";

		if (mEntity == nullptr) mEntity = entity;

		updateViewport();
	}

	Ogre::MovableObject* findMovableObject(Ogre::SceneNode* _node, const Ogre::String& _name)
	{
		for(unsigned short i = 0; i < _node->numAttachedObjects(); i++)
		{
			if(_node->getAttachedObject(i)->getName() == _name)
			{
				return _node->getAttachedObject(i);
			}
		}
		return nullptr;
	}

	Ogre::SceneNode* findSceneNodeObject(Ogre::SceneNode* _node, const Ogre::String& _name)
	{
		for(int i = 0; i < _node->numChildren(); i++)
		{
			if(_node->getChild(i)->getName() == _name)
			{
				return (Ogre::SceneNode*)_node->getChild(i);
			}
		}
		return nullptr;
	}

	void TestRenderBox::removeNode(Ogre::SceneNode* _node)
	{
		//System::Console::WriteLine("remove node {0}", gcnew System::String(_node->getName().c_str()));

		while(_node->numAttachedObjects() != 0)
		{
			Ogre::MovableObject* object = _node->getAttachedObject(0);

			removeEntity(object->getName());
		}

		while (_node->numChildren() != 0)
		{
			Ogre::SceneNode* forDelete = (Ogre::SceneNode*)_node->getChild(0);

			removeNode(forDelete);
		}

		_node->getParentSceneNode()->removeAndDestroyChild(_node->getName());
	}

	void TestRenderBox::removeEntity(const Ogre::String& _name)
	{
		for (VectorEntity::iterator i = mVectorEntity.begin(); i != mVectorEntity.end(); i++)
		{
			if((*i)->getName() == _name)
			{
				//System::Console::WriteLine("entity node {0}", gcnew System::String(_name.c_str()));

				(*i)->getParentSceneNode()->detachObject((*i));
				mScene->destroyMovableObject((*i));

				mVectorEntity.erase(i);
				break;
			}
		}
	}

	void TestRenderBox::synchronizeSceneNode(Ogre::SceneNode* _newNode, Ogre::SceneNode* _fromNode)
	{
		if (_newNode == nullptr || _fromNode == nullptr)
		{
			MYGUI_ASSERT(_newNode == nullptr || _fromNode == nullptr,"Synchronize scene node error.");
			return;
		}

		_newNode->setPosition(_fromNode->getPosition());
		_newNode->setOrientation(_fromNode->getOrientation());

		int i = 0;

		while (i < _newNode->numAttachedObjects())
		{
			Ogre::MovableObject * object = _newNode->getAttachedObject(i);
			Ogre::Entity* entity = object->getMovableType() == "Entity" ? static_cast<Ogre::Entity*>(object) : nullptr;

			if(entity)
			{
				object = findMovableObject(_fromNode, entity->getName());
				Ogre::Entity* oldEntity = (object != nullptr && object->getMovableType() == "Entity") ? static_cast<Ogre::Entity*>(object) : nullptr;

				if(!oldEntity)
				{
					removeEntity(entity->getName());
					continue;
				}
			}
			i++;
		}

		for(i = 0; i < _fromNode->numAttachedObjects(); i++)
		{
			Ogre::MovableObject * object = _fromNode->getAttachedObject(i);
			Ogre::Entity* entity = object->getMovableType() == "Entity" ? static_cast<Ogre::Entity*>(object) : nullptr;

			if(entity)
			{
				object = findMovableObject(_newNode, entity->getName());
				Ogre::Entity* newEntity = (object != nullptr && object->getMovableType() == "Entity") ? static_cast<Ogre::Entity*>(object) : nullptr;

				if(!newEntity)
				{
					//System::Console::WriteLine("create new entity {0}", gcnew System::String(entity->getName().c_str()));

					newEntity = mScene->createEntity(entity->getName(), entity->getMesh()->getName());//new Ogre::Entity(entity->getName(), (Ogre::MeshPtr)entity->getMesh().get()->getHandle());
					_newNode->attachObject(newEntity);

					mVectorEntity.push_back(newEntity);

					if(mEntity == nullptr)
					{
						mEntity = newEntity;
					}
				}
			}
		}

		i = 0;

		while (i < _newNode->numChildren())
		{
			Ogre::SceneNode* oldNode = findSceneNodeObject(_fromNode, _newNode->getChild(i)->getName());

			if(!oldNode)
			{
				Ogre::SceneNode* forDelete = (Ogre::SceneNode*)_newNode->getChild(i);

				removeNode(forDelete);
			}else
			{
				i++;
			}
		}

		for(i = 0; i < _fromNode->numChildren(); i++)
		{
			if(_fromNode->getChild(i)->numChildren() != 0 &&
				((Ogre::SceneNode*)_fromNode->getChild(i))->numAttachedObjects() != 0)
			{
				Ogre::SceneNode* newChildNode = findSceneNodeObject(_newNode, _fromNode->getChild(i)->getName());

				if(!newChildNode)
				{
					//System::Console::WriteLine("create new node {0}", gcnew System::String(_fromNode->getChild(i)->getName().c_str()));
					newChildNode = _newNode->createChildSceneNode(_fromNode->getChild(i)->getName(), _fromNode->getChild(i)->getPosition(), _fromNode->getChild(i)->getOrientation());
				}

				synchronizeSceneNode(newChildNode, (Ogre::SceneNode*)_fromNode->getChild(i));
			}
		}
	}

	bool TestRenderBox::checkSceneNode(Ogre::SceneNode * _sourceNode, Ogre::SceneNode * _node)
	{
		if (_sourceNode == _node) return true;
		for (int i = 0; i < _sourceNode->numChildren(); i++)
		{
			Ogre::Node * node = _sourceNode->getChild(i);
			if (typeid(Ogre::SceneNode) == typeid(*node)) {
				if (checkSceneNode(static_cast<Ogre::SceneNode*>(node), _node)) return true;
			}
			else {
				MYGUI_LOG(Warning, "type Ogre::Node wrong Ogre::SceneNode*");
			}
		}
		return false;
	}

	void TestRenderBox::injectSceneNode(Ogre::SceneManager * _manager, Ogre::SceneNode* _sceneNode)
	{

		// �������
		clear();

		mSceneManagerForSync = _manager;

		// �������� ���� ���� �� ����������
		if (!checkSceneNode(_manager->getRootSceneNode(), _sceneNode)) {
			MYGUI_LOG(Error, "scene node " << _sceneNode << " was deleted");
			return;
		}

		if (mUserViewport) {
			mUserViewport = false;
			createRenderTexture();
		}

		Ogre::SceneNode * node = mNode->createChildSceneNode();

		synchronizeSceneNode(node, _sceneNode);

		mNodeForSync = _sceneNode;

		mPointer = mMouseRotation ? mPointerKeeper : "";

		updateViewport();
	}

	// ������� �����
	void TestRenderBox::clear()
	{
		setRotationAngle(Ogre::Degree(0));

		//if (mEntity) {
			//Ogre::SkeletonManager::getSingleton().remove();
			//mNode->detachObject(mEntity);
		if (mScene)
		{
			mScene->destroyAllEntities();
			mNode->removeAndDestroyAllChildren();
		}
		mVectorEntity.clear();

		mEntity = 0;
		mEntityState = nullptr;

		mSyncTime = 0.0f;
		mNodeForSync = nullptr;
		mSceneManagerForSync = nullptr;
		//}
	}

	void TestRenderBox::setAutoRotationSpeed(int _speed)
	{
		mRotationSpeed = _speed;
	}

	void TestRenderBox::setBackgroungColour(const Ogre::ColourValue & _colour)
	{
		if (false == mUserViewport){
			mBackgroungColour = _colour;
			Ogre::Viewport *v = mRenderTexture->getViewport(0);
			v->setBackgroundColour(mBackgroungColour);
		}
	}

	void TestRenderBox::setRotationAngle(const Ogre::Degree & _rotationAngle)
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

	Ogre::Degree TestRenderBox::getRotationAngle()
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

	void TestRenderBox::setMouseRotation(bool _enable)
	{
		mMouseRotation = _enable;
		mPointer = (mMouseRotation && mEntity) ? mPointerKeeper : "";
	}

	void TestRenderBox::setViewScale(bool _scale)
	{
		if (mUseScale == _scale) return;
		if (needFrameUpdate())
		{
			mUseScale = _scale;
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &TestRenderBox::frameEntered));
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &TestRenderBox::frameEntered);
		}
		else
		{
			mUseScale = _scale;
			if (needFrameUpdate()) Gui::getInstance().eventFrameStart += newDelegate(this, &TestRenderBox::frameEntered);
			//if (needFrameUpdate()) Gui::getInstance().addFrameListener(newDelegate(this, &TestRenderBox::frameEntered), this);
		}
	}

	void TestRenderBox::setRenderTarget(Ogre::Camera * _camera)
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

		if( isTextureCreated() )
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( getTextureName() );

		createTexture( TEXTURE_SIZE, TEXTURE_SIZE, TRM_PT_VIEW_REQUESTED, Ogre::TU_RENDERTARGET );

		mRenderTexture = getBuffer()->getRenderTarget();

		// remove old viewport with 0 z-order
		mRenderTexture->removeViewport( 0 );

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setClearEveryFrame( true );
		mViewport->setOverlaysEnabled(false);
	}

	void TestRenderBox::setPosition(const IntPoint & _point)
	{
		Canvas::setPosition(_point);
	}

	void TestRenderBox::setSize(const IntSize& _size)
	{
		Canvas::setSize(_size);
		updateViewport();
	}

	void TestRenderBox::setCoord(const IntCoord & _coord)
	{
		Canvas::setCoord(_coord);
		updateViewport();
	}

	void TestRenderBox::frameEntered(float _time)
	{
		if (mNodeForSync && mSceneManagerForSync) {
			if (mSyncTime > SYNC_TIMEOUT) {

				if (!checkSceneNode(mSceneManagerForSync->getRootSceneNode(), mNodeForSync)) {
					MYGUI_LOG(Error, "scene node " << mNodeForSync << " was deleted");
					mNodeForSync = nullptr;
					mSceneManagerForSync = nullptr;
					return;
				}

				bool update = false;
				if(mNode->getChild(0)->getPosition() != Ogre::Vector3::ZERO)
				{
					update = true;
				}
				//System::Console::WriteLine("_frameEntered");
				synchronizeSceneNode((Ogre::SceneNode*)mNode->getChild(0), mNodeForSync);
				mNode->getChild(0)->setPosition(Ogre::Vector3::ZERO);
				mNode->getChild(0)->setOrientation(Ogre::Quaternion::IDENTITY);

				if(update)
				{
					updateViewport();
				}
				mSyncTime = 0.0f;
			}

			mSyncTime += _time;
		}

		if ((false == mUserViewport) && (mAutoRotation) && (false == mLeftPressed)) {
			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
			#else
				mNode->yaw(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
			#endif
		}
		if (nullptr != mEntityState) {
			mEntityState->addTime(_time);
		}

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

	void TestRenderBox::onMouseDrag(int _left, int _top)
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
		Widget::onMouseDrag(_left, _top);
	}

	void TestRenderBox::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			if (mMouseRotation/* || mAutoRotation*/) {
				const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
				mLastPointerX = point.left;
				mLeftPressed = true;
			}
		}

		// !!! ����������� �������� � ����� ������
		Widget::onMouseButtonPressed(_left, _top, _id);
	}

	void TestRenderBox::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (MB_Left == _id) mLeftPressed = false;

		// !!! ����������� �������� � ����� ������
		Widget::onMouseButtonReleased(_left, _top, _id);
	}

	void TestRenderBox::createRenderTexture()
	{
		mPointer = mMouseRotation ? mPointerKeeper : "";

		// ������� ����� ���� ��������
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, utility::toString(this, "_SceneManagerTestRenderBox"));

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
		Ogre::Light * light = mScene->createLight(utility::toString(this, "_LightTestRenderBox"));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		createTexture( TEXTURE_SIZE, TEXTURE_SIZE, TRM_PT_VIEW_REQUESTED, Ogre::TU_RENDERTARGET );

		mRenderTexture = getBuffer()->getRenderTarget();

		std::string camera(utility::toString(this, "_CameraTestRenderBox"));
		mRttCam = mScene->createCamera(camera);
		mRttCam->setNearClipDistance(0.00000001);

		mCamNode = mScene->getRootSceneNode()->createChildSceneNode(camera);
		mCamNode->attachObject(mRttCam);
		mRttCam->setNearClipDistance(1);
		if (getHeight() == 0) mRttCam->setAspectRatio(1);
		else mRttCam->setAspectRatio(getWidth()/getHeight());

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setOverlaysEnabled(false);
		mViewport->setClearEveryFrame(true);
		mViewport->setBackgroundColour(mBackgroungColour);
		mViewport->setShadowsEnabled(true);
		mViewport->setSkiesEnabled(false);

		mViewportRect.left = mViewport->getLeft();
		mViewportRect.top = mViewport->getTop();
		mViewportRect.right = mViewport->getLeft() + mViewport->getWidth();
		mViewportRect.bottom = mViewport->getTop() + mViewport->getHeight();
		MYGUI_OUT( "S:", mViewportRect.left, " ", mViewportRect.top, " ", mViewportRect.width(), " ", mViewportRect.height() );
	}

	void TestRenderBox::updateViewport()
	{
		// ��� ���� ��������
		if ((getWidth() <= 1) || (getHeight() <= 1) ) return;

		if ((false == mUserViewport) && (nullptr != mEntity) && (nullptr != mRttCam)) {
			// �� ����, ����� �� ����������� ������, ������������� ������
			mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());

			//System::Console::WriteLine("Width {0}, Height {1}", getWidth(), getHeight());

			// ��������� ����������, ����� ��� ����� ���� ������
			Ogre::AxisAlignedBox box;// = mNode->_getWorldAABB();//mEntity->getBoundingBox();

			VectorEntity::iterator iter = mVectorEntity.begin();

			while (iter != mVectorEntity.end())
			{
				box.merge((*iter)->getBoundingBox().getMinimum() + (*iter)->getParentSceneNode()->_getDerivedPosition());
				box.merge((*iter)->getBoundingBox().getMaximum() + (*iter)->getParentSceneNode()->_getDerivedPosition());
				iter++;
			}

			if (box.isNull()) return;

			//box.scale(Ogre::Vector3(1.41f,1.41f,1.41f));

			//System::Console::WriteLine("Minimum({0}), Maximum({1})",
			//	gcnew System::String(Ogre::StringConverter::toString(box.getMinimum()).c_str()),
			//	gcnew System::String(Ogre::StringConverter::toString(box.getMaximum()).c_str()));

			//box.getCenter();
			Ogre::Vector3 vec = box.getSize();

			// ��������� ��� ������������ ������� ��������� � ���� Z ������������ �����
			#ifdef LEFT_HANDED_CS_UP_Z

				float width = sqrt(vec.x*vec.x + vec.y*vec.y); // ����� ������� - ��������� (���� ������� ������)
				float len2 = width; // mRttCam->getAspectRatio();
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

		if( mViewport != nullptr )
		{
			mViewport->setDimensions( 0, 0, 0.5, 1 );
			return;

			Ogre::Real dwc = (Ogre::Real) getTextureRealWidth() - (Ogre::Real) getWidth();
			Ogre::Real dhc = (Ogre::Real) getTextureRealHeight() - (Ogre::Real) getHeight();

			dwc = -dwc / 2 / mViewport->getActualWidth();
			dhc = -dhc / 2 / mViewport->getActualHeight();

			Ogre::RealRect set;

			set.left = mViewportRect.left + dwc;
			set.top = mViewportRect.top + dhc;
			set.right = set.left + mViewportRect.width() * (Ogre::Real)getWidth() / (Ogre::Real)getTextureRealWidth();
			set.bottom = set.top + mViewportRect.height() * (Ogre::Real)getHeight() / (Ogre::Real)getTextureRealHeight();

#define CHECK_RANGE(v) MYGUI_ASSERT( 0 <= v && v <= 1, "Out of range" );

			CHECK_RANGE( set.left );
			CHECK_RANGE( set.top );
			CHECK_RANGE( set.right );
			CHECK_RANGE( set.bottom );

#undef CHECK_RANGE
			MYGUI_OUT( "N:", set.left, " ", set.top, " ", set.width(), " ", set.height()  );

			mViewport->setDimensions( 
				set.left, set.top,
				set.width(), set.height() );

			mViewport->setClearEveryFrame( true );

			mViewport->_updateDimensions();
		}
	}

	void TestRenderBox::setAutoRotation(bool _auto)
	{
		if (mAutoRotation == _auto) return;
		if (needFrameUpdate())
		{
			mAutoRotation = _auto;
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &TestRenderBox::frameEntered);
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &TestRenderBox::frameEntered));
		}
		else
		{
			mAutoRotation = _auto;
			if (needFrameUpdate()) Gui::getInstance().eventFrameStart += newDelegate(this, &TestRenderBox::frameEntered);
			//if (needFrameUpdate()) Gui::getInstance().addFrameListener(newDelegate(this, &TestRenderBox::frameEntered), this);
		}
	}

	void TestRenderBox::setAnimation(const Ogre::String& _animation)
	{
		if (nullptr != mEntityState) {
			mEntityState = nullptr;
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &TestRenderBox::frameEntered);
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &TestRenderBox::frameEntered));
		}

		if (_animation.empty()) return;

		if (nullptr == mEntity) return;
		Ogre::SkeletonInstance * skeleton = mEntity->getSkeleton();
		if (nullptr == skeleton) return;
		Ogre::AnimationStateSet * anim_set = mEntity->getAllAnimationStates();
		// FIXME ������ ������ ����� ��� ��� ��� �� �������� ��� � ������������������ ����? � ��� �������, �� ���������� ������ ������
		// ��������� ��� getAnimationState - �� ��� ��� ��������� �� ����� � ������� �� �����������
		/*
		Ogre::AnimationState * state = anim_set->getAnimationState(_animation);
		if (state != nullptr)
		{
			// ��� �� ��� ����� ������ ������ ���
		}
		*/
		Ogre::AnimationStateIterator iter = anim_set->getAnimationStateIterator();

		while (iter.hasMoreElements()) {
			Ogre::AnimationState * state = iter.getNext();
			if (_animation == state ->getAnimationName()) {

				// �������������
				Gui::getInstance().eventFrameStart += newDelegate(this, &TestRenderBox::frameEntered);
				//Gui::getInstance().addFrameListener(newDelegate(this, &TestRenderBox::frameEntered), this);

				mEntityState = state;
				mEntityState->setEnabled(true);
				mEntityState->setLoop(true);
				mEntityState->setWeight(1.0f);

				return;
			}
		}
		MYGUI_LOG(Warning, "Unable to to set animation '" << _animation << "' - current entity don't have such animation.");
	}

	void TestRenderBox::onMouseWheel(int _rel)
	{
		if ( ! mUseScale) return;

		const float near_min = 0.5f;
		const float coef = 0.0005;

		mScale += (-_rel) * coef;

		if (mScale > 1) mScale = 1;
		else if (mScale < near_min) mScale = near_min;

	}

	bool TestRenderBox::getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen)
	{
		Ogre::Matrix4 mat = (mRttCam->getProjectionMatrix() * mRttCam->getViewMatrix(true));
		Ogre::Vector4 Point = mat * Ogre::Vector4(_world.x, _world.y, _world.z, 1);
		_screen.x = (Point.x / Point.w + 1) * 0.5;
		_screen.y = 1 - (Point.y / Point.w + 1) * 0.5;
		float Depth = Point.z / Point.w;
		return (Depth >= 0.0f && Depth <= 1.0f);
	}

	void TestRenderBox::updateCanvas( MyGUI::CanvasPtr _canvas )
	{
		mRenderTexture = _canvas->getBuffer()->getRenderTarget();

		// remove old viewport with 0 z-order
		mRenderTexture->removeViewport( 0 );

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setClearEveryFrame( true );
		mViewport->setOverlaysEnabled( false );

		mViewportRect.left = mViewport->getLeft();
		mViewportRect.top = mViewport->getTop();
		mViewportRect.right = mViewport->getLeft() + mViewport->getWidth();
		mViewportRect.bottom = mViewport->getTop() + mViewport->getHeight();
		MYGUI_OUT( "S:", mViewportRect.left, " ", mViewportRect.top, " ", mViewportRect.width(), " ", mViewportRect.height() );

	}


} // namespace MyGUI
