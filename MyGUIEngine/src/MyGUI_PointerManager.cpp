/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	const std::string XML_TYPE("Pointer");

	MYGUI_INSTANCE_IMPLEMENT(PointerManager);

	void PointerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &PointerManager::_load);

		mMousePointer = nullptr;
		mWidgetOwner = nullptr;
		mShow = false;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// удаляем все виджеты
		_destroyAllChildWidget();

		clear();

		WidgetManager::getInstance().unregisterUnlinker(this);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool PointerManager::load(const std::string & _file, const std::string & _group)
	{
		return ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void PointerManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		std::string layer, def, text;

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator pointer = _node->getElementEnumerator();
		while (pointer.next(XML_TYPE)) {

			// парсим атрибуты
			pointer->findAttribute("layer", layer);
			pointer->findAttribute("default", def);

			// сохраняем
			text = pointer->findAttribute("texture");

			IntSize textureSize = SkinManager::getTextureSize(text);

			// берем детей и крутимся, основной цикл
			xml::ElementEnumerator info = pointer->getElementEnumerator();
			while (info.next("Info")) {

				std::string name(info->findAttribute("name"));
				if (mMapPointers.find(name) != mMapPointers.end()) {
					MYGUI_LOG(Warning, "pointer '" << name << "' exist, erase old data");
				}

				std::string resource = info->findAttribute("resource");
				IntSize size = IntSize::parse(info->findAttribute("size"));
				IntPoint point = IntPoint::parse(info->findAttribute("point"));

				//новый вариант курсоров
				if ( ! resource.empty() ) {
					ResourceImageSetPtr image = ResourceManager::getInstance().getResource(resource)->castType<ResourceImageSet>();
					mMapPointers[name] = PointerInfo(point, size, image);

				}
				//старый  вариант курсоров
				else {
					// значения параметров
					FloatRect offset(0, 0, 1, 1);

					// парсим атрибуты
					std::string texture(info->findAttribute("texture"));
					std::string offset_str(info->findAttribute("offset"));
					if (false == offset_str.empty()) {
						if (texture.empty()) offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), textureSize);
						else offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), SkinManager::getTextureSize(texture));
					}

					mMapPointers[name] = PointerInfo(offset, point, size, texture);
				}

			};
		};

		// если есть левел, то пересоеденяем, если нет виджета, то создаем
		if (false == layer.empty()) {
			if (nullptr == mMousePointer) {
				mMousePointer = static_cast<StaticImagePtr>(baseCreateWidget(WidgetStyle::Overlapped, StaticImage::getClassTypeName(), "StaticImage", IntCoord(), Align::Default, "", ""));
			}
			LayerManager::getInstance().attachToLayerKeeper(layer, mMousePointer);
		}

		// если есть дефолтный курсор то меняем
		if (false == def.empty()) mDefaultPointer = def;
		if (false == text.empty()) mTexture = text;

		// если дефолтного нет, то пробуем первый из списка
		if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;

		// ставим дефолтный указатель
		setPointer(mDefaultPointer, nullptr);
	}

	void PointerManager::clear()
	{
		mWidgetOwner = nullptr;
		mDefaultPointer.clear();
		mTexture.clear();
		mMapPointers.clear();
	}

	void PointerManager::setVisible(bool _visible)
	{
		if (nullptr != mMousePointer) mMousePointer->setVisible(_visible);
		mShow = _visible;
	}

	void PointerManager::setPosition(const IntPoint& _pos)
	{
		if (nullptr != mMousePointer) mMousePointer->setPosition(_pos - mPoint);
	}

	void PointerManager::setPointer(const std::string & _name, WidgetPtr _owner)
	{
		if (nullptr == mMousePointer) return;

		MapPointerInfo::iterator iter = mMapPointers.find(_name);
		if (iter == mMapPointers.end()) return;

		// новый вид курсоров через ресурсы
		if (iter->second.resource != nullptr) {
			if (mMousePointer->getItemResource() != iter->second.resource) {
				mMousePointer->setItemResourceInfo(iter->second.resource->getIndexInfo(0, 0));
			}
		}

		// старый вид курсоров
		else {
			// если курсор имеет свой материал
			if (false == iter->second.texture.empty()) {
				if (mMousePointer->_getTextureName() != iter->second.texture) {
					mMousePointer->_setTextureName(iter->second.texture);
				}
				mMousePointer->deleteAllItems();
				mMousePointer->_setUVSet(iter->second.offset);
			}
			else if (false == mTexture.empty()) {
				mMousePointer->deleteAllItems();
				mMousePointer->_setUVSet(iter->second.offset);
				if (mMousePointer->_getTextureName() != mTexture) {
					mMousePointer->_setTextureName(mTexture);
				}
			}
		}

		// сдвигаем с учетом нового и старого смещения
		mMousePointer->setCoord(
			mMousePointer->getLeft() + mPoint.left - iter->second.point.left,
			mMousePointer->getTop() + mPoint.top - iter->second.point.top,
			iter->second.size.width, iter->second.size.height);

		// и сохраняем новое смещение
		mPoint = iter->second.point;
		mWidgetOwner = _owner;
	}

	void PointerManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == mWidgetOwner) setPointer(mDefaultPointer, nullptr);
		else if (_widget == mMousePointer) mMousePointer = nullptr;
	}

	// создает виджет
	WidgetPtr PointerManager::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, _align, nullptr, nullptr, this, _name);
		mWidgetChild.push_back(widget);
		// присоединяем виджет с уровню
		if (false == _layer.empty()) LayerManager::getInstance().attachToLayerKeeper(_layer, widget);
		return widget;
	}

	// удяляет неудачника
	void PointerManager::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end()) {

			// сохраняем указатель
			MyGUI::WidgetPtr widget = *iter;

			// удаляем из списка
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			WidgetManager::getInstance().unlinkFromUnlinkers(_widget);

			// непосредственное удаление
			_deleteWidget(widget);
		}
		else MYGUI_EXCEPT("Widget '" << _widget->getName() << "' not found");
	}

	// удаляет всех детей
	void PointerManager::_destroyAllChildWidget()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		while (false == mWidgetChild.empty()) {

			// сразу себя отписывем, иначе вложенной удаление убивает все
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			manager.unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			_deleteWidget(widget);
		}
	}

} // namespace MyGUI
