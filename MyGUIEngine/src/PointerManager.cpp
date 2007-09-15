
#include "PointerManager.h"
#include "SkinManager.h"
#include "xmlDocument.h"

namespace MyGUI
{

	PointerManager::PointerManager()
	{
		// создаем себе оверлей
//		setOverlay(LayerManager::getInstance().createOverlay());

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		m_overlayElement = static_cast<PanelAlphaOverlayElement *>(overlayManager.createOverlayElement(
			"PanelAlpha", Ogre::StringConverter::toString((int)this) + "_PointerManager" ));
		m_overlayElement->setMetricsMode(Ogre::GMM_PIXELS);

//		layerItem_attachElement(m_overlayElement);
	}

	void PointerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.load(_file)) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return;
		if (xml_root->getName() != "MyGUI_PointerInfo") return;

		// берем детей и крутимся, основной цикл
		xml::VectorNode & pointers = xml_root->getChilds();
		for (size_t i=0; i<pointers.size(); i++) {
			xml::xmlNodePtr pointInfo = pointers[i];
			if (pointInfo->getName() != "Pointer") continue;


			// значения параметров
			std::string layer, material, defaultPointer;
			int size;
			// парсим атрибуты
			const xml::VectorAttributes & attrib = pointInfo->getAttributes();
			for (size_t ia=0; ia<attrib.size(); ia++) {
				// достаем пару атрибут - значение
				const xml::PairAttributes & pairAttributes = attrib[ia];

				if (pairAttributes.first == "Layer") layer = pairAttributes.second;
				else if (pairAttributes.first == "Material") material = pairAttributes.second;
				else if (pairAttributes.first == "Default") defaultPointer = pairAttributes.second;
				else if (pairAttributes.first == "Size") size = parseInt(pairAttributes.second);

			}

			// устанавливаем сразу параметры
			m_overlayElement->setMaterialName(material);
			m_overlayElement->setDimensions(size, size);
			m_defaultPointer = defaultPointer;
			m_layer = layer;
			floatSize materialSize = SkinManager::getMaterialSize(material);

			
			// берем детей и крутимся, основной цикл
			xml::VectorNode & info = pointInfo->getChilds();
			for (size_t j=0; j<info.size(); j++) {
				xml::xmlNodePtr infoInfo = info[j];
				if (infoInfo->getName() != "Info") continue;

				// значения параметров
				std::string name;
				floatRect offset;
				intPoint point;
				// парсим атрибуты
				const xml::VectorAttributes & attrib = infoInfo->getAttributes();
				for (size_t ia=0; ia<attrib.size(); ia++) {
					// достаем пару атрибут - значение
					const xml::PairAttributes & pairAttributes = attrib[ia];

					if (pairAttributes.first == "Name") name = pairAttributes.second;
					else if (pairAttributes.first == "Point") point = intPoint::parse(pairAttributes.second);
					else if (pairAttributes.first == "Offset") offset = SkinManager::convertMaterialCoord(floatRect::parse(pairAttributes.second), materialSize);
				}

				// добавляем курсор
				m_mapPointers[name] = PointerInfo(offset, point);

			}

			// проверяем и инициализируем
			if (m_defaultPointer.empty() && !m_mapPointers.empty()) m_defaultPointer = m_mapPointers.begin()->first;
			// подсоединяем к уровням
			LayerManager::getInstance().attachItem(this, m_layer);

			this->defaultPointer();

			return; // нам нужен только один набор

		} // for (size_t i=0; i<pointers.size(); i++) {
	}

	void PointerManager::clear()
	{
		m_layer.clear();
		m_defaultPointer.clear();
		m_material.clear();
		m_mapPointers.clear();
	}

	void PointerManager::show(bool _show)
	{
		if (m_overlayElement == null) return;
		if (m_show == _show) return;
		m_show = _show;
		m_show ? m_overlayElement->show() : m_overlayElement->hide();
	}

	void PointerManager::move(int _x, int _y)
	{
		if (m_overlayElement == null) return;
		m_overlayElement->setPosition(_x-m_point.left, _y-m_point.top);
	}

	void PointerManager::setPointer(const std::string & _name)
	{
		MapPointerInfo::iterator iter = m_mapPointers.find(_name);
		if (iter == m_mapPointers.end()) return;
		const floatRect & rect = iter->second.offset;
		// сдвигаем с учетом нового и старого смещения
		m_overlayElement->setPosition(m_overlayElement->getLeft()+m_point.left-iter->second.point.left, m_overlayElement->getTop()+m_point.top-iter->second.point.top);
		m_overlayElement->setUV(rect.left, rect.top, rect.right, rect.bottom);
		// и сохраняем новое смещение
		m_point = iter->second.point;
	}

	void PointerManager::shutdown()
	{
		clear();
		// отсоединяем
		LayerManager::getInstance().detachItem(this);
		// удадяем элемент
		if (m_overlayElement != null) {
			Ogre::OverlayManager::getSingleton().destroyOverlayElement(m_overlayElement);
			m_overlayElement = null;
		}
	}

	void PointerManager::attachToOverlay(Ogre::Overlay * _overlay)
	{
		_overlay->add2D(static_cast<Ogre::OverlayContainer*>(m_overlayElement));
	}

	void PointerManager::detachToOverlay(Ogre::Overlay * _overlay)
	{
		_overlay->remove2D(m_overlayElement);
		// пока вручную обнуляем отца
		m_overlayElement->setOverlay(0);
	}

} // namespace MyGUI	