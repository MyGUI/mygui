
#include "LayerManager.h"

namespace MyGUI
{

	LayerManager::LayerManager() {}
	LayerManager::~LayerManager()
	{
		clear();
	}

	bool LayerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.load(path::getFullPath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return false;
		if (xml_root->getName() != "MyGUI_LayerInfo") return false;


		// берем детей и крутимся, основной цикл
		xml::VectorNode & layers = xml_root->getChilds();
		for (size_t i=0; i<layers.size(); i++) {
			xml::xmlNodePtr layerInfo = layers[i];
			if (layerInfo->getName() != "Layer") continue;

			// парсим атрибуты
			const xml::VectorAttributes & attrib = layerInfo->getAttributes();
			std::string name;
			Ogre::ushort start, count, height;
			for (size_t ia=0; ia<attrib.size(); ia++) {
				// достаем пару атрибут - значение
				const xml::PairAttributes & pairAttributes = attrib[ia];
				if (pairAttributes.first == "Name") name = pairAttributes.second;
				else if (pairAttributes.first == "Start") start = parseValue<Ogre::ushort>(pairAttributes.second);
				else if (pairAttributes.first == "Count") count = parseValue<Ogre::ushort>(pairAttributes.second);
				else if (pairAttributes.first == "Height") height = parseValue<Ogre::ushort>(pairAttributes.second);
			}

			// а вот теперь добавляем слой
			m_mapLayer[name] = new LayerInfo(name, start, count, height);

		} // for (size_t i_layer=0; i_layer<layers.size(); i_layer++) {

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
		assert(layer);
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

	LayerItemInfoPtr LayerManager::findItem(int _x, int _y)
	{
		MapLayerSearch::reverse_iterator iter = m_mapLayerSearch.rbegin();
		while (iter != m_mapLayerSearch.rend()) {
			LayerItemInfoPtr item = iter->second->findItem(_x, _y);
			if (item != null) return item;
			iter++;
		}
		return null;
	}

} // namespace MyGUI