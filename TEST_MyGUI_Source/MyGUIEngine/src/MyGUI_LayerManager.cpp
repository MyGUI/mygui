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

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

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
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator layer = _node->getNodeIterator();
		while (layer.nextNode(XML_TYPE)) {

			std::string name, tmp;
			bool overlapped = false;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG(Warning, "Attribute 'name' not found {file : " << _file << "}");
				continue;
			}

			if (layer->findAttribute("overlapped", tmp)) overlapped = utility::parseBool(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'overlapped' not found {file : '" << _file << "' , name : " << name << "}");
			}

			mLayerKeepers.push_back(new LayerKeeper(name, overlapped));
		};
	}

	void LayerManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId) return;

		for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			(*iter)->_render();
		}

	}

	void LayerManager::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	void LayerManager::_unlinkWidget(WidgetPtr _widget)
	{
	}

	void LayerManager::attachToLayerKeeper(const std::string& _name, LayerItem * _item)
	{
		for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			if (_name == (*iter)->getName()) {
				_item->_attachToLayerKeeper(*iter);
				return;
			}
		}
		MYGUI_EXCEPT("Layer '" << _name << "' is not found");
	}

	void LayerManager::detachFromLayerKeeper(LayerItem * _item)
	{
		_item->_detachFromLayerKeeper();
	}

	LayerItem * LayerManager::_findLayerItem(int _left, int _top, LayerItem * _root)
	{
		return null;
	}

	void LayerManager::_upLayerItem(LayerItem * _item)
	{
	}

	void LayerManager::_windowResized(const FloatSize& _size)
	{
		mViewSize = _size;

		for (VectorLayerKeeper::iterator iter=mLayerKeepers.begin(); iter!=mLayerKeepers.end(); ++iter) {
			(*iter)->_resize(mViewSize);
		}
	}

} // namespace MyGUI
