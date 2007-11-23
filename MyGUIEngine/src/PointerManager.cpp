
#include "PointerManager.h"
#include "SkinManager.h"
#include "xmlDocument.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(PointerManager);

	void PointerManager::initialise()
	{
		assert(!mIsInitialise);

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		m_overlayElement = static_cast<PanelAlphaOverlayElement *>(overlayManager.createOverlayElement(
			"PanelAlpha", Ogre::StringConverter::toString((int)this) + "_PointerManager" ));
		m_overlayElement->setMetricsMode(Ogre::GMM_PIXELS);

		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (!mIsInitialise) return;

		clear();
		// отсоединяем
		LayerManager::getInstance().detachItem(this);
		// удадяем элемент
		if (m_overlayElement != null) {
			Ogre::OverlayManager::getSingleton().destroyOverlayElement(m_overlayElement);
			m_overlayElement = null;
		}

		mIsInitialise = false;
	}

	void PointerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if ( (xml_root == 0) || (xml_root->getName() != "MyGUI_PointerInfo") ) return;

		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator pointer = xml_root->getNodeIterator();
		while (pointer.nextNode("Pointer")) {

			// значения параметров
			std::string layer, material, defaultPointer, tmp;
			int size;
			// парсим атрибуты
			pointer->findAttribute("Layer", layer);
			pointer->findAttribute("Material", material);
			pointer->findAttribute("Default", defaultPointer);
			if (pointer->findAttribute("Size", tmp)) size = util::parseInt(tmp);

			// устанавливаем сразу параметры
			m_overlayElement->setMaterialName(material);
			m_overlayElement->setDimensions(size, size);
			m_defaultPointer = defaultPointer;
			m_layer = layer;
			FloatSize materialSize = SkinManager::getMaterialSize(material);

			
			// берем детей и крутимся, основной цикл
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// значения параметров
				std::string name, tmp;
				FloatRect offset;
				IntPoint point;
				// парсим атрибуты

				info->findAttribute("Name", name);
				if (info->findAttribute("Point", tmp)) point = util::parseIntPoint(tmp);
				if (info->findAttribute("Offset", tmp)) offset = SkinManager::convertMaterialCoord(util::parseFloatRect(tmp), materialSize);

				// добавляем курсор
				m_mapPointers[name] = PointerInfo(offset, point);

			};

			// проверяем и инициализируем
			if (m_defaultPointer.empty() && !m_mapPointers.empty()) m_defaultPointer = m_mapPointers.begin()->first;
			// подсоединяем к уровням
			LayerManager::getInstance().attachItem(this, m_layer);

			this->defaultPointer();

			return; // нам нужен только один набор
		};

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
		if (mShow == _show) return;
		mShow = _show;
		mShow ? m_overlayElement->show() : m_overlayElement->hide();
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
		const FloatRect & rect = iter->second.offset;
		// сдвигаем с учетом нового и старого смещения
		m_overlayElement->setPosition(m_overlayElement->getLeft()+m_point.left-iter->second.point.left, m_overlayElement->getTop()+m_point.top-iter->second.point.top);
		m_overlayElement->setUV(rect.left, rect.top, rect.right, rect.bottom);
		// и сохраняем новое смещение
		m_point = iter->second.point;
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