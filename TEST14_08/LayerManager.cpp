
#include "LayerManager.h"

namespace widget
{

	LayerManager::LayerManager() {}

	bool LayerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.load(_file)) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return false;
		if (xml_root->getName() != "MyGUI_LayerInfo") return false;


		// берем детей и крутимся, основной цикл
		xml::VectorNode & layers = xml_root->getChilds();
		for (size_t i_layer=0; i_layer<layers.size(); i_layer++) {
			xml::xmlNodePtr layerInfo = layers[i_layer];
			if (layerInfo->getName() != "Layer") continue;

			// парсим атрибуты
			const xml::VectorAttributes & attrib = layerInfo->getAttributes();
			std::string name;
			Ogre::ushort start, count, height;
			for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
				// достаем пару атрибут - значение
				const xml::PairAttributes & pairAttributes = attrib[i_attrib];
				if (pairAttributes.first == "Name") name = pairAttributes.second;
				else if (pairAttributes.first == "Start") start = parseValue<Ogre::ushort>(pairAttributes.second);
				else if (pairAttributes.first == "Count") count = parseValue<Ogre::ushort>(pairAttributes.second);
				else if (pairAttributes.first == "Height") height = parseValue<Ogre::ushort>(pairAttributes.second);
			}

			// а вот теперь добавляем слой
			m_mapLayer[name] = new LayerInfo(start, count, height);

		} // for (size_t i_layer=0; i_layer<layers.size(); i_layer++) {

		return true;
	}

	void LayerManager::clear()
	{
		for (MapLayer::iterator iter=m_mapLayer.begin();iter!=m_mapLayer.end(); iter++) delete iter->second;
		m_mapLayer.clear();
	}

	void LayerManager::attachItem(LayerItemInfoPtr _item, const std::string & _layer)
	{
		// это наш уровень
		LayerInfoPtr layer = m_mapLayer[_layer];
		assert(layer);
		// запоминаем созданный айтем в виджете
		layer->addItem(_item);
	}

	void LayerManager::detachItem(LayerItemInfoPtr _item)
	{
		// удаляем айтем
		_item->m_layerInfo->removeItem(_item);
	}

	void LayerManager::upItem(LayerItemInfoPtr _item)
	{
		// берем итем уровня и поднимаем
		_item->m_layerInfo->upItem(_item);
	}

	Ogre::Overlay * LayerManager::createOverlay()
	{
		// просто присоединяем элемент к оверлею
		Ogre::Overlay * overlay;
		static long num=0;
		overlay = Ogre::OverlayManager::getSingleton().create(Ogre::StringConverter::toString(num++) + "_LayerManagerOverlay");
		overlay->setZOrder(0);
		overlay->show();
//		overlay->add2D(static_cast<Ogre::OverlayContainer*>(_basis->getOverlayElement()));
		overlay->hide();
		return overlay;
	}


} // namespace widget