/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ISubWidget.h"

#include <OgreTextureManager.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	const float SYNC_TIMEOUT = 1 / 25.0f;
	const size_t TEXTURE_SIZE = 512;

	RenderBox::RenderBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Widget(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mRttCam(null),
		mViewport(null),
		mCamNode(null),
		mUserViewport(false),
		mEntity(null),
		mRotationSpeed(RENDER_BOX_AUTO_ROTATION_SPEED),
		mBackgroungColour(Ogre::ColourValue::Blue),
		mMouseRotation(false),
		mLeftPressed(false),
		mAutoRotation(false),
		mEntityState(null),
		mScale(1.0f),
		mCurrentScale(1.0f),
		mUseScale(false),
		mNodeForSync(null),
		mSceneManagerForSync(null)
	{
		initialiseWidgetSkin(_info);
	}

	RenderBox::~RenderBox()
	{
		//Gui::getInstance().removeFrameListener(newDelegate(this, &RenderBox::frameEntered));
		mTexture.setNull();
		clear();

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);

		shutdownWidgetSkin();
	}

	void RenderBox::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void RenderBox::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(null != mMainSkin, "need one subskin");

		// сохраняем оригинальный курсор
		mPointerKeeper = mPointer;
		mPointer.clear();

		mMainSkin->_setUVSet(FloatRect(0, 0, 1, 1));
		createRenderTexture();
	}

	void RenderBox::shutdownWidgetSkin()
	{
	}

	// добавляет в сцену объект, старый удаляеться
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

	Ogre::MovableObject* findMovableObject(Ogre::SceneNode* _node, const Ogre::String& _name)
	{
		for(unsigned short i = 0; i < _node->numAttachedObjects(); i++)
		{
			if(_node->getAttachedObject(i)->getName() == _name)
			{
				return _node->getAttachedObject(i);
			}
		}
		return null;
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
		return null;
	}

	void RenderBox::removeNode(Ogre::SceneNode* _node)
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

	void RenderBox::removeEntity(const Ogre::String& _name)
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

	void RenderBox::synchronizeSceneNode(Ogre::SceneNode* _newNode, Ogre::SceneNode* _fromNode)
	{
		if (_newNode == null || _fromNode == null)
		{
			MYGUI_ASSERT(_newNode == null || _fromNode == null,"Synchronize scene node error.");
			return;
		}

		_newNode->setPosition(_fromNode->getPosition());
		_newNode->setOrientation(_fromNode->getOrientation());

		int i = 0;

		while (i < _newNode->numAttachedObjects())
		{
			Ogre::MovableObject * object = _newNode->getAttachedObject(i);
			Ogre::Entity* entity = object->getMovableType() == "Entity" ? static_cast<Ogre::Entity*>(object) : null;

			if(entity)
			{
				object = findMovableObject(_fromNode, entity->getName());
				Ogre::Entity* oldEntity = (object != null && object->getMovableType() == "Entity") ? static_cast<Ogre::Entity*>(object) : null;

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
			Ogre::Entity* entity = object->getMovableType() == "Entity" ? static_cast<Ogre::Entity*>(object) : null;

			if(entity)
			{
				object = findMovableObject(_newNode, entity->getName());
				Ogre::Entity* newEntity = (object != null && object->getMovableType() == "Entity") ? static_cast<Ogre::Entity*>(object) : null;

				if(!newEntity)
				{
					//System::Console::WriteLine("create new entity {0}", gcnew System::String(entity->getName().c_str()));

					newEntity = mScene->createEntity(entity->getName(), entity->getMesh()->getName());//new Ogre::Entity(entity->getName(), (Ogre::MeshPtr)entity->getMesh().get()->getHandle());
					_newNode->attachObject(newEntity);

					mVectorEntity.push_back(newEntity);

					if(mEntity == null)
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

	bool RenderBox::checkSceneNode(Ogre::SceneNode * _sourceNode, Ogre::SceneNode * _node)
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

	void RenderBox::injectSceneNode(Ogre::SceneManager * _manager, Ogre::SceneNode* _sceneNode)
	{

		// очищаем
		clear();

		mSceneManagerForSync = _manager;

		// проверка сцен нода на валидность
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

	// очищает сцену
	void RenderBox::clear()
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
		mEntityState = null;

		mSyncTime = 0.0f;
		mNodeForSync = null;
		mSceneManagerForSync = null;
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
			Ogre::Viewport *v = mRenderTexture->getViewport(0);
			v->setBackgroundColour(mBackgroungColour);
		}
	}

	void RenderBox::setRotationAngle(const Ogre::Degree & _rotationAngle)
	{
		if (false == mUserViewport) {
			mNode->resetOrientation();
			// коррекция под левосторонюю систему координат с осью Z направленную вверх
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
			// коррекция под левосторонюю систему координат с осью Z направленную вверх
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
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &RenderBox::frameEntered));
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &RenderBox::frameEntered);
		}
		else
		{
			mUseScale = _scale;
			if (needFrameUpdate()) Gui::getInstance().eventFrameStart += newDelegate(this, &RenderBox::frameEntered);
			//if (needFrameUpdate()) Gui::getInstance().addFrameListener(newDelegate(this, &RenderBox::frameEntered), this);
		}
	}

	void RenderBox::setRenderTarget(Ogre::Camera * _camera)
	{
		// полная очистка
		clear();
		mPointer = "";

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);
		mScene = 0;

		// создаем новый материал
		mUserViewport = true;
		mRttCam = _camera;

		std::string texture(utility::toString(this, "_TextureRenderBox"));

		Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture(texture);

		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		manager.remove(texture);

		mTexture = manager.createManual(texture, MyGUI::ResourceManager::getInstance().getResourceGroup(),
			Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
		mRenderTexture = mTexture->getBuffer()->getRenderTarget();

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setClearEveryFrame(true);

		_setTextureName(texture);
	}

	void RenderBox::setPosition(const IntPoint & _point)
	{
		Widget::setPosition(_point);
	}

	void RenderBox::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		updateViewport();
	}

	void RenderBox::setCoord(const IntCoord & _coord)
	{
		Widget::setCoord(_coord);
		updateViewport();
	}

	void RenderBox::frameEntered(float _time)
	{
		if (mNodeForSync && mSceneManagerForSync) {
			if (mSyncTime > SYNC_TIMEOUT) {

				if (!checkSceneNode(mSceneManagerForSync->getRootSceneNode(), mNodeForSync)) {
					MYGUI_LOG(Error, "scene node " << mNodeForSync << " was deleted");
					mNodeForSync = null;
					mSceneManagerForSync = null;
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
			// коррекция под левосторонюю систему координат с осью Z направленную вверх
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
			#else
				mNode->yaw(Ogre::Radian(Ogre::Degree(_time * mRotationSpeed)));
			#endif
		}
		if (null != mEntityState) {
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

	void RenderBox::onMouseDrag(int _left, int _top)
	{
		if ((false == mUserViewport) && mMouseRotation/* && mAutoRotation*/) {
			// коррекция под левосторонюю систему координат с осью Z направленную вверх
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			#else
				mNode->yaw(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			#endif
			mLastPointerX = _left;
		}

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseDrag(_left, _top);
	}

	void RenderBox::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (_id == MB_Left) {
			if (mMouseRotation/* || mAutoRotation*/) {
				const IntPoint & point = InputManager::getInstance().getLastLeftPressed();
				mLastPointerX = point.left;
				mLeftPressed = true;
			}
		}

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseButtonPressed(_left, _top, _id);
	}

	void RenderBox::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (MB_Left == _id) mLeftPressed = false;

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::onMouseButtonReleased(_left, _top, _id);
	}

	void RenderBox::createRenderTexture()
	{
		mPointer = mMouseRotation ? mPointerKeeper : "";

		// создаем новый сцен менеджер
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, utility::toString(this, "_SceneManagerRenderBox"));

		// создаем нод к которуму будем всякую дрянь атачить
		mNode = mScene->getRootSceneNode()->createChildSceneNode();

		mScene->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		// главный источник света
		// коррекция под левосторонюю систему координат с осью Z направленную вверх
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
		mTexture = manager.createManual(texture, MyGUI::ResourceManager::getInstance().getResourceGroup(),
			Ogre::TEX_TYPE_2D, TEXTURE_SIZE, TEXTURE_SIZE, 0, Ogre::PF_B8G8R8A8, Ogre::TU_RENDERTARGET);
		mRenderTexture = mTexture->getBuffer()->getRenderTarget();

		std::string camera(utility::toString(this, "_CameraRenderBox"));
		mRttCam = mScene->createCamera(camera);
		mRttCam->setNearClipDistance(0.00000001);

		mCamNode = mScene->getRootSceneNode()->createChildSceneNode(camera);
		mCamNode->attachObject(mRttCam);
		mRttCam->setNearClipDistance(1);
		if (getHeight() == 0) mRttCam->setAspectRatio(1);
		else mRttCam->setAspectRatio(getWidth()/getHeight());

		mViewport = mRenderTexture->addViewport( mRttCam );
		mViewport->setOverlaysEnabled(false);
		mViewport->setClearEveryFrame( true );
		mViewport->setBackgroundColour(mBackgroungColour);
		mViewport->setShadowsEnabled(true);
		mViewport->setSkiesEnabled(false);

		_setTextureName(texture);
	}

	void RenderBox::updateViewport()
	{
		// при нуле вылетает
		if ((getWidth() <= 1) || (getHeight() <= 1) ) return;

		if ((false == mUserViewport) && (null != mEntity) && (null != mRttCam)) {
			// не ясно, нужно ли растягивать камеру, установленную юзером
			mRttCam->setAspectRatio((float)getWidth() / (float)getHeight());

			//System::Console::WriteLine("Width {0}, Height {1}", getWidth(), getHeight());

			// вычисляем расстояние, чтобы был виден весь объект
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

			// коррекция под левосторонюю систему координат с осью Z направленную вверх
			#ifdef LEFT_HANDED_CS_UP_Z

				float width = sqrt(vec.x*vec.x + vec.y*vec.y); // самое длинное - диагональ (если крутить модель)
				float len2 = width; // mRttCam->getAspectRatio();
				float height = vec.z;
				float len1 = height;
				if (len1 < len2) len1 = len2;
				len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view
				// центр объекта по вертикали + отъехать так, чтобы влезла ближняя грань BoundingBox'а + чуть вверх и еще назад для красоты
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

				float width = sqrt(vec.x*vec.x + vec.z*vec.z); // самое длинное - диагональ (если крутить модель)
				float len2 = width / mRttCam->getAspectRatio();
				float height = vec.y;
				float len1 = height;
				if (len1 < len2) len1 = len2;
				len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view
				// центр объекта по вертикали + отъехать так, чтобы влезла ближняя грань BoundingBox'а + чуть вверх и еще назад для красоты
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
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &RenderBox::frameEntered);
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &RenderBox::frameEntered));
		}
		else
		{
			mAutoRotation = _auto;
			if (needFrameUpdate()) Gui::getInstance().eventFrameStart += newDelegate(this, &RenderBox::frameEntered);
			//if (needFrameUpdate()) Gui::getInstance().addFrameListener(newDelegate(this, &RenderBox::frameEntered), this);
		}
	}

	void RenderBox::setAnimation(const Ogre::String& _animation)
	{
		if (null != mEntityState) {
			mEntityState = null;
			if (needFrameUpdate() == false) Gui::getInstance().eventFrameStart -= newDelegate(this, &RenderBox::frameEntered);
			//if (needFrameUpdate() == false) Gui::getInstance().removeFrameListener(newDelegate(this, &RenderBox::frameEntered));
		}

		if (_animation.empty()) return;

		if (null == mEntity) return;
		Ogre::SkeletonInstance * skeleton = mEntity->getSkeleton();
		if (null == skeleton) return;
		Ogre::AnimationStateSet * anim_set = mEntity->getAllAnimationStates();
		// FIXME почему вместо всего что под ним не написать как в закомментированнои коде? я его добавил, но протестить немогу просто
		// посмотрел код getAnimationState - он как раз проверяет по имени с которым ты сравниваешь
		/*
		Ogre::AnimationState * state = anim_set->getAnimationState(_animation);
		if (state != null)
		{
			// тут то что стоит внутри твоего ифа
		}
		*/
		Ogre::AnimationStateIterator iter = anim_set->getAnimationStateIterator();

		while (iter.hasMoreElements()) {
			Ogre::AnimationState * state = iter.getNext();
			if (_animation == state ->getAnimationName()) {

				// подписываемся
				Gui::getInstance().eventFrameStart += newDelegate(this, &RenderBox::frameEntered);
				//Gui::getInstance().addFrameListener(newDelegate(this, &RenderBox::frameEntered), this);

				mEntityState = state;
				mEntityState->setEnabled(true);
				mEntityState->setLoop(true);
				mEntityState->setWeight(1.0f);

				return;
			}
		}
		MYGUI_LOG(Warning, "Unable to to set animation '" << _animation << "' - current entity don't have such animation.");
	}

	void RenderBox::onMouseWheel(int _rel)
	{
		if ( ! mUseScale) return;

		const float near_min = 0.5f;
		const float coef = 0.0005;

		mScale += (-_rel) * coef;

		if (mScale > 1) mScale = 1;
		else if (mScale < near_min) mScale = near_min;

	}

	bool RenderBox::getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen)
	{
		Ogre::Matrix4 mat = (mRttCam->getProjectionMatrix() * mRttCam->getViewMatrix(true));
		Ogre::Vector4 Point = mat * Ogre::Vector4(_world.x, _world.y, _world.z, 1);
		_screen.x = (Point.x / Point.w + 1) * 0.5;
		_screen.y = 1 - (Point.y / Point.w + 1) * 0.5;
		float Depth = Point.z / Point.w;
		return (Depth >= 0.0f && Depth <= 1.0f);
	}

} // namespace MyGUI
