/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_PointerManager.h"
#include "MyGUI_SkinManager.h"
#include "xmlDocument.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(PointerManager);

	void PointerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
		mOverlayElement = static_cast<PanelAlphaOverlayElement *>(overlayManager.createOverlayElement(
			"PanelAlpha", Ogre::StringConverter::toString((int)this) + "_PointerManager" ));
		mOverlayElement->setMetricsMode(Ogre::GMM_PIXELS);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		clear();
		// �����������
		LayerManager::getInstance().detachItem(this);
		// ������� �������
		if (mOverlayElement != null) {
			Ogre::OverlayManager::getSingleton().destroyOverlayElement(mOverlayElement);
			mOverlayElement = null;
		}

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	bool PointerManager::load(const std::string & _file, bool _resource)
	{
		clear();

		xml::xmlDocument doc;
		std::string file = (_resource ? helper::getResourcePath(_file) : _file).c_str();
		if ("" == file) {
			MYGUI_ERROR("Pointer: " + _file + " not found");
			return false;
		}
		if (false == doc.open(file)) {
			MYGUI_ERROR("Pointer: " + doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			MYGUI_ERROR("Pointer: " + _file + " root tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if ( (false == root->findAttribute("type", type)) || (type != "Pointer") ) {
			MYGUI_ERROR("Pointer: " + _file + " root type 'Pointer' not found");
			return false;
		}

		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator pointer = root->getNodeIterator();
		while (pointer.nextNode("Pointer")) {

			// �������� ����������
			std::string layer, material, defaultPointer, tmp;
			int size;
			// ������ ��������
			pointer->findAttribute("layer", layer);
			pointer->findAttribute("material", material);
			pointer->findAttribute("default", defaultPointer);
			if (pointer->findAttribute("size", tmp)) size = util::parseInt(tmp);

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

				info->findAttribute("name", name);
				if (info->findAttribute("point", tmp)) point = IntPoint::parse(tmp);
				if (info->findAttribute("offset", tmp)) offset = SkinManager::convertMaterialCoord(FloatRect::parse(tmp), materialSize);

				// ��������� ������
				mMapPointers[name] = PointerInfo(offset, point);

			};

			// ��������� � ��������������
			if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;
			// ������������ � �������
			LayerManager::getInstance().attachItem(this, mLayer);

			this->defaultPointer();

			return true; // ��� ����� ������ ���� �����
		};

		return true;
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

	void PointerManager::setPosition(const IntPoint& _pos)
	{
		if (mOverlayElement == null) return;
		mOverlayElement->setPosition(_pos.left - mPoint.left, _pos.top - mPoint.top);
	}

	void PointerManager::setPointer(const std::string & _name, WidgetPtr _owner)
	{
		MapPointerInfo::iterator iter = mMapPointers.find(_name);
		if (iter == mMapPointers.end()) return;
		const FloatRect & rect = iter->second.offset;
		// �������� � ������ ������ � ������� ��������
		mOverlayElement->setPosition(mOverlayElement->getLeft()+mPoint.left-iter->second.point.left, mOverlayElement->getTop()+mPoint.top-iter->second.point.top);
		mOverlayElement->setUV(rect.left, rect.top, rect.right, rect.bottom);
		// � ��������� ����� ��������
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
		// ���� ������� �������� ����
		mOverlayElement->setOverlay(0);
	}

} // namespace MyGUI	