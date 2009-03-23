/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "RenderBoxWrap.h"

namespace wraps
{

	const float SYNC_TIMEOUT = 1 / 25.0f;
	const size_t TEXTURE_SIZE = 512;

	RenderBoxWrap::RenderBoxWrap(MyGUI::RenderBoxPtr _box) :
		mEntity(nullptr),
		mRttCam(nullptr),
		mCamNode(nullptr),
		mRotationSpeed(RENDER_BOX_AUTO_ROTATION_SPEED),
		mMouseRotation(false),
		mLeftPressed(false),
		mAutoRotation(false),
		mEntityState(nullptr),
		mScale(1.0f),
		mCurrentScale(1.0f),
		mUseScale(false),
		mNodeForSync(nullptr),
		mSceneManagerForSync(nullptr),
		mRenderBox(_box),
		mFrameAdvise(false)
	{
		createScene();
		mRenderBox->eventMouseDrag = newDelegate(this, &RenderBoxWrap::notifyMouseDrag);
		mRenderBox->eventMouseWheel = newDelegate(this, &RenderBoxWrap::notifyMouseWheel);
		mRenderBox->eventMouseButtonPressed = newDelegate(this, &RenderBoxWrap::notifyMouseButtonPressed);
		mRenderBox->eventMouseButtonReleased = newDelegate(this, &RenderBoxWrap::notifyMouseButtonReleased);
		mRenderBox->eventUpdateViewport = newDelegate(this, &RenderBoxWrap::notifyUpdateViewport);
	}

	RenderBoxWrap::~RenderBoxWrap()
	{
		FrameAdvise(false);
		mRenderBox->removeCamera();

		mRenderBox->eventMouseDrag = nullptr;
		mRenderBox->eventMouseWheel = nullptr;
		mRenderBox->eventMouseButtonPressed = nullptr;
		mRenderBox->eventMouseButtonReleased = nullptr;
		mRenderBox->eventUpdateViewport = nullptr;

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root && mScene) root->destroySceneManager(mScene);
	}

	// добавл€ет в сцену объект, старый удал€етьс€
	void RenderBoxWrap::injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position, const Ogre::Quaternion & _orientation, const Ogre::Vector3 & _scale)
	{

		static size_t num = 0;

		Ogre::Entity * entity = mScene->createEntity(MyGUI::utility::toString(this, "_RenderBoxMesh_", _meshName, num++), _meshName);
		Ogre::SceneNode * node = mNode->createChildSceneNode(_position, _orientation);
		node->attachObject(entity);
		mVectorEntity.push_back(entity);

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

	void RenderBoxWrap::removeNode(Ogre::SceneNode* _node)
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

	void RenderBoxWrap::removeEntity(const Ogre::String& _name)
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

	void RenderBoxWrap::synchronizeSceneNode(Ogre::SceneNode* _newNode, Ogre::SceneNode* _fromNode)
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

	bool RenderBoxWrap::checkSceneNode(Ogre::SceneNode * _sourceNode, Ogre::SceneNode * _node)
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

	void RenderBoxWrap::injectSceneNode(Ogre::SceneManager * _manager, Ogre::SceneNode* _sceneNode)
	{

		// очищаем
		clearScene();

		mSceneManagerForSync = _manager;

		// проверка сцен нода на валидность
		if (!checkSceneNode(_manager->getRootSceneNode(), _sceneNode)) {
			MYGUI_LOG(Error, "scene node " << _sceneNode << " was deleted");
			return;
		}

		Ogre::SceneNode * node = mNode->createChildSceneNode();

		synchronizeSceneNode(node, _sceneNode);

		mNodeForSync = _sceneNode;

		updateViewport();
	}

	// очищает сцену
	void RenderBoxWrap::clearScene()
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

	void RenderBoxWrap::setAutoRotationSpeed(int _speed)
	{
		mRotationSpeed = _speed;
	}

	void RenderBoxWrap::setRotationAngle(const Ogre::Degree & _rotationAngle)
	{
		mNode->resetOrientation();
		// коррекци€ под левосторонюю систему координат с осью Z направленную вверх
		#ifdef LEFT_HANDED_CS_UP_Z
			mNode->roll(Ogre::Radian(_rotationAngle));
		#else
			mNode->yaw(Ogre::Radian(_rotationAngle));
		#endif
	}

	Ogre::Degree RenderBoxWrap::getRotationAngle()
	{
		// коррекци€ под левосторонюю систему координат с осью Z направленную вверх
		#ifdef LEFT_HANDED_CS_UP_Z
			return Ogre::Degree(mNode->getOrientation().getRoll());
		#else
			return Ogre::Degree(mNode->getOrientation().getYaw());
		#endif
		return Ogre::Degree(0);
	}

	void RenderBoxWrap::setMouseRotation(bool _enable)
	{
		mMouseRotation = _enable;
	}

	void RenderBoxWrap::setViewScale(bool _scale)
	{
		if (mUseScale == _scale) return;
		if (needFrameUpdate())
		{
			mUseScale = _scale;
			FrameAdvise(false);
		}
		else
		{
			mUseScale = _scale;
			FrameAdvise(true);
		}
	}

	void RenderBoxWrap::frameEntered(float _time)
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

		if (mAutoRotation && !mLeftPressed) {
			// коррекци€ под левосторонюю систему координат с осью Z направленную вверх
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

	void RenderBoxWrap::createScene()
	{
		// создаем новый сцен менеджер
		mScene = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, MyGUI::utility::toString(this, "_SceneManagerRenderBox"));

		// создаем нод к которуму будем вс€кую др€нь атачить
		mNode = mScene->getRootSceneNode()->createChildSceneNode();

		mScene->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

		// главный источник света
		// коррекци€ под левосторонюю систему координат с осью Z направленную вверх
		#ifdef LEFT_HANDED_CS_UP_Z
			Ogre::Vector3 dir(10, 10, -10);
		#else
			Ogre::Vector3 dir(-1, -1, 0.5);
		#endif

		dir.normalise();
		Ogre::Light * light = mScene->createLight(MyGUI::utility::toString(this, "_LightRenderBox"));
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		std::string camera(MyGUI::utility::toString(this, "_CameraRenderBox"));
		mRttCam = mScene->createCamera(camera);
		mRttCam->setNearClipDistance(1);

		mCamNode = mScene->getRootSceneNode()->createChildSceneNode(camera);
		mCamNode->attachObject(mRttCam);

		if (mRenderBox->getHeight() == 0) mRttCam->setAspectRatio(1);
		else mRttCam->setAspectRatio( float(mRenderBox->getWidth()) / float(mRenderBox->getHeight()) );

		mRenderBox->setCamera(mRttCam);
		mRenderBox->setBackgroungColour(Ogre::ColourValue::ZERO);
	}

	void RenderBoxWrap::updateViewport()
	{
		// при нуле вылетает
		if ((mRenderBox->getWidth() <= 1) || (mRenderBox->getHeight() <= 1) ) return;

		if ((nullptr != mEntity) && (nullptr != mRttCam)) {
			// не €сно, нужно ли раст€гивать камеру, установленную юзером
			mRttCam->setAspectRatio((float)mRenderBox->getWidth() / (float)mRenderBox->getHeight());

			//System::Console::WriteLine("Width {0}, Height {1}", getWidth(), getHeight());

			// вычисл€ем рассто€ние, чтобы был виден весь объект
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

			// коррекци€ под левосторонюю систему координат с осью Z направленную вверх
			#ifdef LEFT_HANDED_CS_UP_Z

				float width = sqrt(vec.x*vec.x + vec.y*vec.y); // самое длинное - диагональ (если крутить модель)
				float len2 = width; // mRttCam->getAspectRatio();
				float height = vec.z;
				float len1 = height;
				if (len1 < len2) len1 = len2;
				len1 /= 0.86; // [sqrt(3)/2] for 60 degrees field of view
				// центр объекта по вертикали + отъехать так, чтобы влезла ближн€€ грань BoundingBox'а + чуть вверх и еще назад дл€ красоты
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
				// центр объекта по вертикали + отъехать так, чтобы влезла ближн€€ грань BoundingBox'а + чуть вверх и еще назад дл€ красоты
				Ogre::Vector3 result = box.getCenter() + Ogre::Vector3(0, 0, vec.z/2 + len1) + Ogre::Vector3(0, height*0.1, len1*0.2);
				result.z *= mCurrentScale;
				Ogre::Vector3 look = Ogre::Vector3(0, box.getCenter().y /*+ box.getCenter().y * (1-mCurrentScale)*/, 0);

				mCamNode->setPosition(result);
				mCamNode->lookAt(look, Ogre::Node::TS_WORLD);

			#endif
		}
	}

	void RenderBoxWrap::setAutoRotation(bool _auto)
	{
		if (mAutoRotation == _auto) return;
		if (needFrameUpdate())
		{
			mAutoRotation = _auto;
			FrameAdvise(false);
		}
		else
		{
			mAutoRotation = _auto;
			FrameAdvise(true);
		}
	}

	void RenderBoxWrap::setAnimation(const Ogre::String& _animation)
	{
		if (nullptr != mEntityState) {
			mEntityState = nullptr;
			FrameAdvise(false);
		}

		if (_animation.empty()) return;

		if (nullptr == mEntity) return;
		Ogre::SkeletonInstance * skeleton = mEntity->getSkeleton();
		if (nullptr == skeleton) return;
		Ogre::AnimationStateSet * anim_set = mEntity->getAllAnimationStates();
		// FIXME почему вместо всего что под ним не написать как в закомментированнои коде? € его добавил, но протестить немогу просто
		// посмотрел код getAnimationState - он как раз провер€ет по имени с которым ты сравниваешь
		/*
		Ogre::AnimationState * state = anim_set->getAnimationState(_animation);
		if (state != nullptr)
		{
			// тут то что стоит внутри твоего ифа
		}
		*/
		Ogre::AnimationStateIterator iter = anim_set->getAnimationStateIterator();

		while (iter.hasMoreElements()) {
			Ogre::AnimationState * state = iter.getNext();
			if (_animation == state ->getAnimationName()) {

				// подписываемс€
				FrameAdvise(true);

				mEntityState = state;
				mEntityState->setEnabled(true);
				mEntityState->setLoop(true);
				mEntityState->setWeight(1.0f);

				return;
			}
		}
		MYGUI_LOG(Warning, "Unable to to set animation '" << _animation << "' - current entity don't have such animation.");
	}

	bool RenderBoxWrap::getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen)
	{
		Ogre::Matrix4 mat = (mRttCam->getProjectionMatrix() * mRttCam->getViewMatrix(true));
		Ogre::Vector4 Point = mat * Ogre::Vector4(_world.x, _world.y, _world.z, 1);
		_screen.x = (Point.x / Point.w + 1) * 0.5;
		_screen.y = 1 - (Point.y / Point.w + 1) * 0.5;
		float Depth = Point.z / Point.w;
		return (Depth >= 0.0f && Depth <= 1.0f);
	}

	void RenderBoxWrap::notifyUpdateViewport(MyGUI::RenderBoxPtr _sender)
	{
		updateViewport();
	}

	void RenderBoxWrap::FrameAdvise(bool _advise)
	{
		if (_advise && !mFrameAdvise && needFrameUpdate())
		{
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &RenderBoxWrap::frameEntered);
			mFrameAdvise = true;
		}
		else if (!_advise && mFrameAdvise && !needFrameUpdate())
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &RenderBoxWrap::frameEntered);
			mFrameAdvise = false;
		}
	}

	void RenderBoxWrap::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		if (mMouseRotation) {
			// коррекци€ под левосторонюю систему координат с осью Z направленную вверх
			#ifdef LEFT_HANDED_CS_UP_Z
				mNode->roll(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			#else
				mNode->yaw(Ogre::Radian(Ogre::Degree(_left - mLastPointerX)));
			#endif
			mLastPointerX = _left;
		}
	}

	void RenderBoxWrap::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left) {
			if (mMouseRotation) {
				const MyGUI::IntPoint & point = MyGUI::InputManager::getInstance().getLastLeftPressed();
				mLastPointerX = point.left;
				mLeftPressed = true;
			}
		}
	}

	void RenderBoxWrap::notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (MyGUI::MouseButton::Left == _id) mLeftPressed = false;
	}

	void RenderBoxWrap::notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		if ( ! mUseScale) return;

		const float near_min = 0.5f;
		const float coef = 0.0005;

		mScale += (-_rel) * coef;

		if (mScale > 1) mScale = 1;
		else if (mScale < near_min) mScale = near_min;

	}

} // namespace wraps
