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
#include "MyGUI_ResourceManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layer");

	INSTANCE_IMPLEMENT(LayerManager);

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayerManager::_load);

		Ogre::SceneManagerEnumerator::SceneManagerIterator iter = Ogre::Root::getSingleton().getSceneManagerIterator();
		if (iter.hasMoreElements()) {
			mSceneManager = iter.getNext();
			mSceneManager->addRenderQueueListener(this);
		}
		else {
			mSceneManager = null;
		}

		// �������������
		mPixScaleX = mPixScaleY = 1;
        mHOffset = mVOffset = 0;
		mAspectCoef = 1;
		mUpdate = false;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;

		// �� �������, � ������� ����������� ���
		// ��� �����?
		mMaximumDepth = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();
	}

	void LayerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// ������� ��� ��������� �����
		clear();

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

	void LayerManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		VectorLayerKeeperPtr layers;
		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator layer = _node->getNodeIterator();
		while (layer.nextNode(XML_TYPE)) {

			std::string name;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG(Warning, "Attribute 'name' not found (file : " << _file << ")");
				continue;
			}

			for (VectorLayerKeeperPtr::iterator iter=layers.begin(); iter!=layers.end(); ++iter) {
				MYGUI_ASSERT((*iter)->getName() != name, "Layer '" << name << "' already exist (file : " << _file << ")");
			}

			layers.push_back(new LayerKeeper(name,
				utility::parseBool(layer->findAttribute("overlapped")),
				utility::parseBool(layer->findAttribute("peek")) ));
		};

		// ������ ������ ����� � ������ ����
		merge(layers);
	}

	void LayerManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		Ogre::Viewport * vp = mSceneManager->getCurrentViewport();
		if ((null == vp) || (false == vp->getOverlaysEnabled())) return;

		mCountBatch = 0;
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			(*iter)->_render(mUpdate);
		}

		// ���������� ����
		mUpdate = false;

	}

	void LayerManager::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	void LayerManager::_unlinkWidget(WidgetPtr _widget)
	{
		detachFromLayerKeeper(_widget);
	}

	// ��������� �� ������ � ��������� �� ���������
	void LayerManager::attachToLayerKeeper(const std::string& _name, WidgetPtr _item)
	{
		MYGUI_ASSERT(_item->isRootWidget(), "attached widget must be root");

		// ������� �����������
		detachFromLayerKeeper(_item);

		// � ������ �������
		for (VectorLayerKeeperPtr::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if (_name == (*iter)->getName()) {

				// ���������� � ������� ������� ��������� ������
				_item->mLayerKeeper = (*iter);

				// ������� �� ��������� ���� ��� ����
				_item->mLayerItemKeeper = (*iter)->getItem();

				// ������������� �� �������
				_item->mLayerItemKeeper->_addPeekItem(_item);

				// ��������� ������������ ��������
				_item->_attachToLayerItemKeeper(_item->mLayerItemKeeper);

				return;
			}
		}
		MYGUI_EXCEPT("Layer '" << _name << "' is not found");
	}

	void LayerManager::detachFromLayerKeeper(WidgetPtr _item)
	{
		MYGUI_ASSERT(null != _item, "pointer must be valid");

		// �� ��� ���������� � �����
		if (null == _item->mLayerKeeper) return;

		// ������������ �� ��������
		_item->mLayerItemKeeper->_removePeekItem(_item);

		// ��� ������ ����������
		LayerItemKeeper * save = _item->mLayerItemKeeper;

		// ��������� ����������� 
		_item->_detachFromLayerItemKeeper();

		// ����������� ���� � �������� � �������� �������
		_item->mLayerKeeper->leaveItem(save);
		_item->mLayerItemKeeper = null;
		_item->mLayerKeeper = null;
	}

	LayerItem * LayerManager::_findLayerItem(int _left, int _top, LayerItem* &_root)
	{
		VectorLayerKeeperPtr::reverse_iterator iter = mLayerKeepers.rbegin();
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

		// ���������� �� ����
		while (_item->getParent() != null) _item = _item->getParent();

		// ���� �����������, �� ���������
		if (null != _item->mLayerKeeper) _item->mLayerKeeper->upItem(_item->mLayerItemKeeper);

	}

	void LayerManager::_windowResized(const IntSize& _size)
	{
		// ����� ������
		mPixScaleX = 1.0 / _size.width;
		mPixScaleY = 1.0 / _size.height;
		mAspectCoef = float(_size.height) / _size.width;

		Ogre::RenderSystem * render = Ogre::Root::getSingleton().getRenderSystem();

        mHOffset = render->getHorizontalTexelOffset() / _size.width;
        mVOffset = render->getVerticalTexelOffset() / _size.height;

		// �������� ����
		mUpdate = true;

	}

	void LayerManager::setSceneManager(Ogre::SceneManager * _scene)
	{
		if (null != mSceneManager) mSceneManager->removeRenderQueueListener(this);
		mSceneManager = _scene;
		if (null != mSceneManager) mSceneManager->addRenderQueueListener(this);
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
			if ((*iter) == null) continue;
			bool find = false;
			std::string name = (*iter)->getName();
			for (VectorLayerKeeperPtr::iterator iter2=_layers.begin(); iter2!=_layers.end(); ++iter2) {
				if (name == (*iter2)->getName()) {
					// �������� ����� ����, �� ��� ������������
					delete (*iter2);
					(*iter2) = (*iter);
					(*iter) = null;
					find = true;
					break;
				}
			}
			if (!find) {
				destroy(*iter);
				(*iter) = null;
			}
		}

		// ������ � ��������
		mLayerKeepers = _layers;
	}

	void LayerManager::destroy(LayerKeeperPtr _layer)
	{
		MYGUI_LOG(Info, "destroy layer '" << _layer->getName() << "'");
		delete _layer;
	}

} // namespace MyGUI
