/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(LayerManager);

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		clear();

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	bool LayerManager::load(const std::string & _file, bool _resource)
	{
		clear();

		xml::xmlDocument doc;
		if (false == doc.open((_resource ? helper::getResourcePath(_file) : _file).c_str())) {
			MYGUI_ERROR(doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			MYGUI_ERROR("not find root tag 'MyGUI'");
			return false;
		}

		std::string type;
		if ( (false == root->findAttribute("type", type)) || (type != "Layer") ) {
			MYGUI_ERROR("not find root type 'Layer'");
			return false;
		}

		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator layer = root->getNodeIterator();
		while (layer.nextNode("Layer")) {

			std::string name, tmp;
			Ogre::ushort start = 0, count = 1, height = 1;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG("Attribute 'name' not find {file : ", _file, "}");
				continue;
			}

			if (layer->findAttribute("height", tmp)) height = util::parseUShort(tmp);
			else MYGUI_LOG("Attribute 'Height' not find {file : '", _file, "' , name : ", name, "}");

			if (layer->findAttribute("count", tmp)) count = util::parseUShort(tmp);
			else MYGUI_LOG("Attribute 'Count' not find {file : '", _file, "' , name : ", name, "}");

			if (layer->findAttribute("start", tmp)) start = util::parseUShort(tmp);
			else MYGUI_LOG("Attribute 'Start' not find {file : '", _file, "' , name : ", name, "}");

			// а вот теперь добавляем слой
			mMapLayer[name] = new LayerInfo(name, start, count, height);

		};

		return true;
	}

	void LayerManager::clear()
	{
		for (MapLayer::iterator iter=mMapLayer.begin();iter!=mMapLayer.end(); iter++) delete iter->second;
		mMapLayer.clear();
	}

	void LayerManager::attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch)
	{
		// это наш уровень
		LayerInfoPtr layer = mMapLayer[_layer];
		if (layer == null) MYGUI_EXCEPT(_layer + " - no find level");
		// запоминаем созданный айтем в виджете
		layer->addItem(_item);
		// добавляем уровень в карту поиска
		if (_attachToSearch) {
			MapLayerSearch::iterator iter = mMapLayerSearch.find(layer->mStart);
			// если не нашли такого то добавляем
			if (iter == mMapLayerSearch.end()) mMapLayerSearch[layer->mStart] = layer;
		}
	}

	void LayerManager::detachItem(LayerItemInfoPtr _item)
	{
		// удаляем айтем если он приаттачен
		if (_item->mLayerInfo) _item->mLayerInfo->removeItem(_item);
	}

	void LayerManager::upItem(LayerItemInfoPtr _item)
	{
		// берем итем уровня и поднимаем
		if (_item->mLayerInfo) _item->mLayerInfo->upItem(_item);
	}

	LayerItemInfoPtr LayerManager::findWidgetItem(int _left, int _top, LayerItemInfoPtr & _rootItem)
	{
		MapLayerSearch::reverse_iterator iter = mMapLayerSearch.rbegin();
		while (iter != mMapLayerSearch.rend()) {
			LayerItemInfoPtr item = iter->second->findItem(_left, _top, _rootItem);
			if (item != null) {
				if ( ! item->isWidget() ) {
					_rootItem = null;
					return null;
				}
				return item;
			}
			iter++;
		}
		return null;
	}

} // namespace MyGUI