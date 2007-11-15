
#include "LayerManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(LayerManager);

	LayerManager::LayerManager() {}
	LayerManager::~LayerManager()
	{
		clear();
	}

	bool LayerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if ( (xml_root == 0) || (xml_root->getName() != "MyGUI_LayerInfo") ) return false;


		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator layer = xml_root->getNodeIterator();
		while (layer.nextNode("Layer")) {

			std::string name, tmp;
			Ogre::ushort start = 0, count = 1, height = 1;

			if ( false == layer->findAttribute("Name", name)) {
				LOG("Attribute 'Name' not find {file : ", _file, "}");
				continue;
			}

			if (layer->findAttribute("Height", tmp)) height = util::parseUShort(tmp);
			else LOG("Attribute 'Height' not find {file : '", _file, "' , Name : ", name, "}");

			if (layer->findAttribute("Count", tmp)) count = util::parseUShort(tmp);
			else LOG("Attribute 'Count' not find {file : '", _file, "' , Name : ", name, "}");

			if (layer->findAttribute("Start", tmp)) start = util::parseUShort(tmp);
			else LOG("Attribute 'Start' not find {file : '", _file, "' , Name : ", name, "}");

			// а вот теперь добавляем слой
			m_mapLayer[name] = new LayerInfo(name, start, count, height);

		};

		return true;
	}

	void LayerManager::clear()
	{
		for (MapLayer::iterator iter=m_mapLayer.begin();iter!=m_mapLayer.end(); iter++) delete iter->second;
		m_mapLayer.clear();
	}

	void LayerManager::attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch)
	{
		// это наш уровень
		LayerInfoPtr layer = m_mapLayer[_layer];
		if (layer == null) OGRE_EXCEPT(0, _layer + " - no find level", "LayerManager::attachItem");
		// запоминаем созданный айтем в виджете
		layer->addItem(_item);
		// добавляем уровень в карту поиска
		if (_attachToSearch) {
			MapLayerSearch::iterator iter = m_mapLayerSearch.find(layer->m_start);
			// если не нашли такого то добавляем
			if (iter == m_mapLayerSearch.end()) m_mapLayerSearch[layer->m_start] = layer;
		}
	}

	void LayerManager::detachItem(LayerItemInfoPtr _item)
	{
		// удаляем айтем если он приаттачен
		if (_item->m_layerInfo) _item->m_layerInfo->removeItem(_item);
	}

	void LayerManager::upItem(LayerItemInfoPtr _item)
	{
		// берем итем уровня и поднимаем
		if (_item->m_layerInfo) _item->m_layerInfo->upItem(_item);
	}

	LayerItemInfoPtr LayerManager::findWidgetItem(int _x, int _y, LayerItemInfoPtr & _rootItem)
	{
		MapLayerSearch::reverse_iterator iter = m_mapLayerSearch.rbegin();
		while (iter != m_mapLayerSearch.rend()) {
			LayerItemInfoPtr item = iter->second->findItem(_x, _y, _rootItem);
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