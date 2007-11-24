
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
		mOverlayElement = static_cast<PanelAlphaOverlayElement *>(overlayManager.createOverlayElement(
			"PanelAlpha", Ogre::StringConverter::toString((int)this) + "_PointerManager" ));
		mOverlayElement->setMetricsMode(Ogre::GMM_PIXELS);

		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (!mIsInitialise) return;

		clear();
		// �����������
		LayerManager::getInstance().detachItem(this);
		// ������� �������
		if (mOverlayElement != null) {
			Ogre::OverlayManager::getSingleton().destroyOverlayElement(mOverlayElement);
			mOverlayElement = null;
		}

		mIsInitialise = false;
	}

	void PointerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) MYGUI_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if ( (xml_root == 0) || (xml_root->getName() != "MyGUI_PointerInfo") ) return;

		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator pointer = xml_root->getNodeIterator();
		while (pointer.nextNode("Pointer")) {

			// �������� ����������
			std::string layer, material, defaultPointer, tmp;
			int size;
			// ������ ��������
			pointer->findAttribute("Layer", layer);
			pointer->findAttribute("Material", material);
			pointer->findAttribute("Default", defaultPointer);
			if (pointer->findAttribute("Size", tmp)) size = util::parseInt(tmp);

			// ������������� ����� ���������
			mOverlayElement->setMaterialName(material);
			mOverlayElement->setDimensions(size, size);
			mDefaultPointer = defaultPointer;
			mLayer = layer;
			FloatSize materialSize = SkinManager::getMaterialSize(material);

			
			// ����� ����� � ��������, �������� ����
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// �������� ����������
				std::string name, tmp;
				FloatRect offset;
				IntPoint point;
				// ������ ��������

				info->findAttribute("Name", name);
				if (info->findAttribute("Point", tmp)) point = util::parseIntPoint(tmp);
				if (info->findAttribute("Offset", tmp)) offset = SkinManager::convertMaterialCoord(util::parseFloatRect(tmp), materialSize);

				// ��������� ������
				mMapPointers[name] = PointerInfo(offset, point);

			};

			// ��������� � ��������������
			if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;
			// ������������ � �������
			LayerManager::getInstance().attachItem(this, mLayer);

			this->defaultPointer();

			return; // ��� ����� ������ ���� �����
		};

	}

	void PointerManager::clear()
	{
		mLayer.clear();
		mDefaultPointer.clear();
		mMaterial.clear();
		mMapPointers.clear();
	}

	void PointerManager::show(bool _show)
	{
		if (mOverlayElement == null) return;
		if (mShow == _show) return;
		mShow = _show;
		mShow ? mOverlayElement->show() : mOverlayElement->hide();
	}

	void PointerManager::move(int _left, int _top)
	{
		if (mOverlayElement == null) return;
		mOverlayElement->setPosition(_left-mPoint.left, _top-mPoint.top);
	}

	void PointerManager::setPointer(const std::string & _name)
	{
		MapPointerInfo::iterator iter = mMapPointers.find(_name);
		if (iter == mMapPointers.end()) return;
		const FloatRect & rect = iter->second.offset;
		// �������� � ������ ������ � ������� ��������
		mOverlayElement->setPosition(mOverlayElement->getLeft()+mPoint.left-iter->second.point.left, mOverlayElement->getTop()+mPoint.top-iter->second.point.top);
		mOverlayElement->setUV(rect.left, rect.top, rect.right, rect.bottom);
		// � ��������� ����� ��������
		mPoint = iter->second.point;
	}

	void PointerManager::attachToOverlay(Ogre::Overlay * _overlay)
	{
		_overlay->add2D(static_cast<Ogre::OverlayContainer*>(mOverlayElement));
	}

	void PointerManager::detachToOverlay(Ogre::Overlay * _overlay)
	{
		_overlay->remove2D(mOverlayElement);
		// ���� ������� �������� ����
		mOverlayElement->setOverlay(0);
	}

} // namespace MyGUI	