/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	const std::string XML_TYPE("Pointer");

	INSTANCE_IMPLEMENT(PointerManager);

	void PointerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &PointerManager::_load);

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		mOverlayElement = static_cast<SharedPanelAlphaOverlayElement *>(overlayManager.createOverlayElement(
			"SharedPanelAlpha", Ogre::StringConverter::toString((size_t)this) + "_PointerManager" ));
		// устанавливаем колличество саб оверлеев
		mOverlayElement->setCountSharedOverlay(1);

		mOverlayElement->setMetricsMode(Ogre::GMM_PIXELS);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		clear();
		// отсоединяем
		LayerManager::getInstance().detachItem(this);
		// удадяем элемент
		if (mOverlayElement != null) {
			Ogre::OverlayManager::getSingleton().destroyOverlayElement(mOverlayElement);
			mOverlayElement = null;
		}

		WidgetManager::getInstance().unregisterUnlinker(this);
		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool PointerManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void PointerManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator pointer = _node->getNodeIterator();
		while (pointer.nextNode(XML_TYPE)) {

			// значения параметров
			std::string layer, defaultPointer;

			// парсим атрибуты
			pointer->findAttribute("layer", layer);
			pointer->findAttribute("default", defaultPointer);

			// сохраняем
			mMaterial = pointer->findAttribute("material");
			mSize = IntSize::parse(pointer->findAttribute("size"));

			// устанавливаем сразу параметры
			mOverlayElement->setMaterialName(mMaterial);
			mOverlayElement->setDimensionInfo(mSize.width, mSize.height, 0);
			if (false == defaultPointer.empty()) mDefaultPointer = defaultPointer;
			mLayer = layer;
			FloatSize materialSize = SkinManager::getMaterialSize(mMaterial);


			// берем детей и крутимся, основной цикл
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// значения параметров
				FloatRect offset(0, 0, 1, 1);

				// парсим атрибуты
				std::string material(info->findAttribute("material"));
				std::string name(info->findAttribute("name"));
				std::string size(info->findAttribute("size"));
				IntPoint point = IntPoint::parse(info->findAttribute("point"));

				std::string offset_str(info->findAttribute("offset"));
				if (false == offset_str.empty()) {
					if (material.empty()) offset = SkinManager::convertMaterialCoord(FloatRect::parse(offset_str), materialSize);
					else offset = SkinManager::convertMaterialCoord(FloatRect::parse(offset_str), SkinManager::getMaterialSize(material));
				}

				// добавляем курсор
				mMapPointers[name] = PointerInfo(offset, point, IntSize::parse(size), material);

			};
		};

		// проверяем и инициализируем
		if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;
		// подсоединяем к уровням
		LayerManager::getInstance().attachItem(this, mLayer);

		this->defaultPointer();
	}

	void PointerManager::clear()
	{
		mLayer.clear();
		mDefaultPointer.clear();
		mMaterial.clear();
		mMapPointers.clear();
	}

	void PointerManager::show()
	{
		if (mOverlayElement == null) return;
		if (mOverlayElement->isVisible()) return;
		mOverlayElement->show();
	}

	void PointerManager::hide()
	{
		if (mOverlayElement == null) return;
		if (false == mOverlayElement->isVisible()) return;
		mOverlayElement->hide();
	}

	void PointerManager::setPosition(const IntPoint& _pos)
	{
		if (mOverlayElement == null) return;
		mOverlayElement->setPositionInfo(_pos.left - mPoint.left, _pos.top - mPoint.top, 0);
	}

	void PointerManager::setPointer(const std::string & _name, WidgetPtr _owner)
	{
		MapPointerInfo::iterator iter = mMapPointers.find(_name);
		if (iter == mMapPointers.end()) return;
		const FloatRect & rect = iter->second.offset;

		// если курсор имеет свой материал
		if (false == iter->second.material.empty()) {
			if (mOverlayElement->getMaterialName() != iter->second.material) mOverlayElement->setMaterialName(iter->second.material);
		}
		else if (false == mMaterial.empty()) {
			if (mOverlayElement->getMaterialName() != mMaterial) mOverlayElement->setMaterialName(mMaterial);
		}

		// если курсор имеет свой размер
		IntSize size(mSize);
		if (0 < iter->second.size.width) size = iter->second.size;

		// сдвигаем с учетом нового и старого смещения
		mOverlayElement->setPositionInfo(mOverlayElement->getLeft()+mPoint.left-iter->second.point.left, mOverlayElement->getTop()+mPoint.top-iter->second.point.top, size.width, size.height, 0);
		mOverlayElement->setUVInfo(rect.left, rect.top, rect.right, rect.bottom, 0);
		// и сохраняем новое смещение
		mPoint = iter->second.point;
		mWidgetOwner = _owner;
	}

	void PointerManager::attachToOverlay(Ogre::Overlay * _overlay)
	{
		_overlay->add2D(static_cast<Ogre::OverlayContainer*>(mOverlayElement));
	}

	void PointerManager::detachToOverlay(Ogre::Overlay * _overlay)
	{
		_overlay->remove2D(mOverlayElement);
		// пока вручную обнуляем отца
		mOverlayElement->setOverlay(0);
	}

	LayerItemInfoPtr PointerManager::findItem(int _left, int _top)
	{
		return null;
	}

	void PointerManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == mWidgetOwner) defaultPointer();
	}

} // namespace MyGUI	
