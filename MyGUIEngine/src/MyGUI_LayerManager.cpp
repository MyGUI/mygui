/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_LayerKeeper.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layer");

	MYGUI_INSTANCE_IMPLEMENT(LayerManager);

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayerManager::_load);

		// инициализация
		mSceneManager = nullptr;
		mPixScaleX = mPixScaleY = 1;
        mHOffset = mVOffset = 0;
		mAspectCoef = 1;
		mUpdate = false;
		mMaximumDepth = 0;

		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr) {
			Ogre::SceneManagerEnumerator::SceneManagerIterator iter = root->getSceneManagerIterator();
			if (iter.hasMoreElements()) {
				mSceneManager = iter.getNext();
				mSceneManager->addRenderQueueListener(this);
			}

			// подписываемся на рендер евент
			Ogre::RenderSystem * render = root->getRenderSystem();
			if (render != nullptr)
			{
				render->addListener(this);
				// не забывай, о великий построитель гуёв
				// Кто здесь?
				mMaximumDepth = render->getMaximumDepthInputValue();
			}
		}

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// удаляем подписку на рендер евент
		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr) {
			root->getRenderSystem()->removeListener(this);
		}

		// удаляем все хранители слоев
		clear();

		setSceneManager(nullptr);

		WidgetManager::getInstance().unregisterUnlinker(this);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void LayerManager::clear()
	{
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			destroy(*iter);
		}
		mLayerKeepers.clear();
	}

	bool LayerManager::load(const std::string & _file, const std::string & _group)
	{
		return ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LayerManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		VectorLayerKeeperPtr layers;
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator layer = _node->getElementEnumerator();
		while (layer.next(XML_TYPE)) {

			std::string name;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG(Warning, "Attribute 'name' not found (file : " << _file << ")");
				continue;
			}

			for (VectorLayerKeeperPtr::iterator iter=layers.begin(); iter!=layers.end(); ++iter) {
				MYGUI_ASSERT((*iter)->getName() != name, "Layer '" << name << "' already exist (file : " << _file << ")");
			}

			bool pick = false;
			// если версия меньше 1.0 то переименовываем стейты
			if (_version < Version(1, 0)) {
				pick = utility::parseBool(layer->findAttribute("peek"));
			}
			else {
				pick = utility::parseBool(layer->findAttribute("pick"));
			}

			layers.push_back(new LayerKeeper(name, utility::parseBool(layer->findAttribute("overlapped")), pick));
		};

		// теперь мержим новые и старые слои
		merge(layers);
	}

	void LayerManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		Ogre::Viewport * vp = mSceneManager->getCurrentViewport();
		if ((nullptr == vp) || (false == vp->getOverlaysEnabled())) return;

		mCountBatch = 0;
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			(*iter)->_render(mUpdate);
		}

		// сбрасываем флаг
		mUpdate = false;
	}

	void LayerManager::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	void LayerManager::_unlinkWidget(WidgetPtr _widget)
	{
		detachFromLayerKeeper(_widget);
	}

	// поправить на виджет и проверять на рутовость
	void LayerManager::attachToLayerKeeper(const std::string& _name, WidgetPtr _item)
	{
		MYGUI_ASSERT(_item->isRootWidget(), "attached widget must be root");

		// сначала отсоединяем
		detachFromLayerKeeper(_item);

		// а теперь аттачим
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if (_name == (*iter)->getName()) {

				// запоминаем в рутовом виджете хранитель лееров
				_item->mLayerKeeper = (*iter);

				// достаем из хранителя леер для себя
				_item->mLayerItemKeeper = (*iter)->createItem();

				// подписываемся на пиккинг
				_item->mLayerItemKeeper->_addLayerItem(_item);

				// физически подсоединяем иерархию
				_item->_attachToLayerItemKeeper(_item->mLayerItemKeeper, true);

				return;
			}
		}
		MYGUI_EXCEPT("Layer '" << _name << "' is not found");
	}

	void LayerManager::detachFromLayerKeeper(WidgetPtr _item)
	{
		MYGUI_ASSERT(nullptr != _item, "pointer must be valid");

		// мы уже отдетачены в доску
		if (nullptr == _item->mLayerKeeper) return;

		// такого быть не должно
		MYGUI_ASSERT(_item->mLayerItemKeeper, "_item->mLayerItemKeeper == nullptr");

		// отписываемся от пиккинга
		_item->mLayerItemKeeper->_removeLayerItem(_item);

		// при детаче обнулиться
		LayerItemKeeper * save = _item->mLayerItemKeeper;

		// физически отсоединяем
		_item->_detachFromLayerItemKeeper(true);

		// отсоединяем леер и обнуляем у рутового виджета
		_item->mLayerKeeper->destroyItem(save);
		_item->mLayerItemKeeper = nullptr;
		_item->mLayerKeeper = nullptr;
	}

	void LayerManager::upLayerItem(WidgetPtr _item)
	{
		LayerItemKeeper * item = _item ? _item->getLayerItemKeeper() : nullptr;
		if (item) item->upItem();
	}

	void LayerManager::_windowResized(const IntSize& _size)
	{
		// новый размер
		mPixScaleX = 1.0 / _size.width;
		mPixScaleY = 1.0 / _size.height;
		mAspectCoef = float(_size.height) / _size.width;

		Ogre::RenderSystem * render = Ogre::Root::getSingleton().getRenderSystem();

        mHOffset = render->getHorizontalTexelOffset() / _size.width;
        mVOffset = render->getVerticalTexelOffset() / _size.height;

		// обновить всех
		mUpdate = true;
	}

	void LayerManager::setSceneManager(Ogre::SceneManager * _scene)
	{
		if (nullptr != mSceneManager) mSceneManager->removeRenderQueueListener(this);
		mSceneManager = _scene;
		if (nullptr != mSceneManager) mSceneManager->addRenderQueueListener(this);
	}

	bool LayerManager::isExist(const std::string & _name)
	{
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if (_name == (*iter)->getName()) return true;
		}
		return false;
	}

	void LayerManager::merge(VectorLayerKeeperPtr & _layers)
	{
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if ((*iter) == nullptr) continue;
			bool find = false;
			std::string name = (*iter)->getName();
			for (VectorLayerKeeperPtr::iterator iter2=_layers.begin(); iter2!=_layers.end(); ++iter2) {
				if (name == (*iter2)->getName()) {
					// заменяем новый слой, на уже существующий
					delete (*iter2);
					(*iter2) = (*iter);
					(*iter) = nullptr;
					find = true;
					break;
				}
			}
			if (!find) {
				destroy(*iter);
				(*iter) = nullptr;
			}
		}

		// теперь в основной
		mLayerKeepers = _layers;
	}

	void LayerManager::destroy(LayerKeeperPtr _layer)
	{
		MYGUI_LOG(Info, "destroy layer '" << _layer->getName() << "'");
		delete _layer;
	}

	bool LayerManager::isExistItem(LayerItemKeeper * _item)
	{
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if ((*iter)->existItem(_item)) return true;
		}
		return false;
	}

	void LayerManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
	{
		if(eventName == "DeviceLost")
		{
		}
		else if(eventName == "DeviceRestored")
		{
			// обновить всех
			mUpdate = true;
		}
	}

	WidgetPtr LayerManager::getWidgetFromPoint(int _left, int _top)
	{
		VectorLayerKeeperPtr::reverse_iterator iter = mLayerKeepers.rbegin();
		while (iter != mLayerKeepers.rend()) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top);
			if (item != nullptr) return static_cast<WidgetPtr>(item);
			++iter;
		}
		return nullptr;
	}

} // namespace MyGUI
