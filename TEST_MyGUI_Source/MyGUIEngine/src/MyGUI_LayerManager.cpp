/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_Common.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_LayerKeeper.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layer");

	INSTANCE_IMPLEMENT(LayerManager);

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayerManager::_load);

		Ogre::SceneManager * mSceneManager = Ogre::Root::getSingleton().getSceneManagerIterator().getNext();
		mSceneManager->addRenderQueueListener(this);

		// инициализаци€
		mPixScaleX = mPixScaleY = 1;
        mHOffset = mVOffset = 0;
		mAspectCoef = 1;
		mUpdate = false;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// удал€ем все хранители слоев
		for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			delete (*iter);
		}
		mLayerKeepers.clear();

		WidgetManager::getInstance().unregisterUnlinker(this);
		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool LayerManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LayerManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// берем детей и крутимс€, основной цикл
		xml::xmlNodeIterator layer = _node->getNodeIterator();
		while (layer.nextNode(XML_TYPE)) {

			std::string name;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG(Warning, "Attribute 'name' not found {file : " << _file << "}");
				continue;
			}

			mLayerKeepers.push_back(new LayerKeeper(name,
				utility::parseBool(layer->findAttribute("overlapped")),
				utility::parseBool(layer->findAttribute("peek")) ));
		};
	}

	void LayerManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
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

	// поправить на виджет и провер€ть на рутовость
	void LayerManager::attachToLayerKeeper(const std::string& _name, WidgetPtr _item)
	{
		MYGUI_ASSERT(_item->isRootWidget(), "attached widget must be root");

		// сначала отсоедин€ем
		detachFromLayerKeeper(_item);

		// а теперь аттачим
		for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if (_name == (*iter)->getName()) {

				// запоминаем в рутовом виджете хранитель лееров
				_item->mLayerKeeper = (*iter);

				// достаем из хранител€ леер дл€ себ€
				_item->mLayerItemKeeper = (*iter)->getItem();

				// подписываемс€ на пиккинг
				_item->mLayerItemKeeper->_addPeekItem(_item);

				// физически подсоедин€ем иерархию
				_item->_attachToLayerItemKeeper(_item->mLayerItemKeeper);

				return;
			}
		}
		MYGUI_EXCEPT("Layer '" << _name << "' is not found");
	}

	void LayerManager::detachFromLayerKeeper(WidgetPtr _item)
	{
		MYGUI_ASSERT(null != _item, "pointer must be valid");

		// мы уже отдетачены в доску
		if (null == _item->mLayerKeeper) return;

		// отписываемс€ от пиккинга
		_item->mLayerItemKeeper->_removePeekItem(_item);

		// при детаче обнулитьс€
		LayerItemKeeper * save = _item->mLayerItemKeeper;

		// физически отсоедин€ем 
		_item->_detachFromLayerItemKeeper();

		// отсоедин€ем леер и обнул€ем у рутового виджета
		_item->mLayerKeeper->leaveItem(save);
		_item->mLayerItemKeeper = null;
		_item->mLayerKeeper = null;
	}

	LayerItem * LayerManager::_findLayerItem(int _left, int _top, LayerItem* &_root)
	{
		VectorLayerKeeper::reverse_iterator iter = mLayerKeepers.rbegin();
		while (iter != mLayerKeepers.rend()) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top, _root);
			if (item != null) return item;
			++iter;
		}
		return null;
	}

	void LayerManager::upLayerItem(WidgetPtr _item)
	{
		if (null == _item) return;

		// добираемс€ до рута
		while (_item->getParent() != null) _item = _item->getParent();

		// если приаттачены, то поднимаем
		if (null != _item->mLayerKeeper) _item->mLayerKeeper->upItem(_item->mLayerItemKeeper);

	}

	void LayerManager::_windowResized(const FloatSize& _size)
	{
		// новый размер
		mViewSize = _size;

		mPixScaleX = 1.0 / _size.width;
		mPixScaleY = 1.0 / _size.height;
		mAspectCoef = _size.height / _size.width;

		Ogre::RenderSystem * render = Ogre::Root::getSingleton().getRenderSystem();

        mHOffset = render->getHorizontalTexelOffset() / _size.width;
        mVOffset = render->getVerticalTexelOffset() / _size.height;

		// обновить всех
		mUpdate = true;

		/*for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			(*iter)->_resize(mViewSize);
		}*/
	}

} // namespace MyGUI
