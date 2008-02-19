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

		mMousePointer = null;
		mWidgetOwner = null;
		mShow = false;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		clear();

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
		std::string layer, def, text;

		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator pointer = _node->getNodeIterator();
		while (pointer.nextNode(XML_TYPE)) {

			// парсим атрибуты
			pointer->findAttribute("layer", layer);
			pointer->findAttribute("default", def);

			// сохраняем
			text = pointer->findAttribute("texture");

			FloatSize textureSize = SkinManager::getTextureSize(text);

			// берем детей и крутимся, основной цикл
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// значения параметров
				FloatRect offset(0, 0, 1, 1);

				// парсим атрибуты
				std::string texture(info->findAttribute("texture"));
				std::string name(info->findAttribute("name"));
				std::string size(info->findAttribute("size"));
				IntPoint point = IntPoint::parse(info->findAttribute("point"));

				std::string offset_str(info->findAttribute("offset"));
				if (false == offset_str.empty()) {
					if (texture.empty()) offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), textureSize);
					else offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), SkinManager::getTextureSize(texture));
				}

				// добавляем курсор
				if (mMapPointers.find(name) != mMapPointers.end()) {
					MYGUI_LOG(Warning, "pointer '" << name << "' exist, erase old data");
				}
				mMapPointers[name] = PointerInfo(offset, point, IntSize::parse(size), texture);

			};
		};

		// если есть левел, то пересоеденяем, если нет виджета, то создаем
		if (false == layer.empty()) {
			if (null == mMousePointer) {
				mMousePointer = WidgetManager::getInstance().createWidget("Widget", "DefaultClient", IntCoord(), ALIGN_DEFAULT, null, "");
				//WidgetManager::getInstance().unlinkFromUnlinkers(mMousePointer);
			}
			LayerManager::getInstance().attachToLayerKeeper(layer, mMousePointer);
		}

		// если есть дефолтный курсор то меняем
		if (false == def.empty()) mDefaultPointer = def;
		if (false == text.empty()) mTexture = text;

		// если дефолтного нет, то пробуем первый из списка
		if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;

		// ставим дефолтный указатель
		setPointer(mDefaultPointer, null);
	}

	void PointerManager::clear()
	{
		if (null != mMousePointer) {
			WidgetManager::getInstance()._deleteWidget(mMousePointer);
			mMousePointer = null;
		}
		mWidgetOwner = null;
		mDefaultPointer.clear();
		mTexture.clear();
		mMapPointers.clear();
	}

	void PointerManager::show()
	{
		if (null != mMousePointer) mMousePointer->show();
		mShow = true;
	}

	void PointerManager::hide()
	{
		if (null != mMousePointer) mMousePointer->hide();
		mShow = false;
	}

	void PointerManager::setPosition(const IntPoint& _pos)
	{
		if (null != mMousePointer) mMousePointer->setPosition(_pos - mPoint);
	}

	void PointerManager::setPointer(const std::string & _name, WidgetPtr _owner)
	{
		if (null == mMousePointer) return;

		MapPointerInfo::iterator iter = mMapPointers.find(_name);
		if (iter == mMapPointers.end()) return;
		const FloatRect & rect = iter->second.offset;

		// если курсор имеет свой материал
		if (false == iter->second.texture.empty()) {
			//if (mOverlayElement->getMaterialName() != iter->second.material) mOverlayElement->setMaterialName(iter->second.material);
		}
		else if (false == mTexture.empty()) {
			//if (mOverlayElement->getMaterialName() != mMaterial) mOverlayElement->setMaterialName(mMaterial);
		}

		// сдвигаем с учетом нового и старого смещения
		if (null != mMousePointer) {
			mMousePointer->setPosition(mMousePointer->getLeft()+mPoint.left-iter->second.point.left, mMousePointer->getTop()+mPoint.top-iter->second.point.top, iter->second.size.width, iter->second.size.height);
			mMousePointer->_setUVSet(rect);
		}

		// и сохраняем новое смещение
		mPoint = iter->second.point;
		mWidgetOwner = _owner;
	}

	void PointerManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == mWidgetOwner) setPointer(mDefaultPointer, null);
		//else if (_widget == mMousePointer) mMousePointer = null;
	}

} // namespace MyGUI	
