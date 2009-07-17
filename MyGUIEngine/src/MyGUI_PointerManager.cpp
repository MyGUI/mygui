/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
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
#include "MyGUI_CoordConverter.h"
#include "MyGUI_TextureManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FactoryManager.h"

#include "MyGUI_ResourceManualPointer.h"
#include "MyGUI_ResourceImageSetPointer.h"

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

		FactoryManager::getInstance().registryFactory<ResourceManualPointer>(XML_TYPE);
		FactoryManager::getInstance().registryFactory<ResourceImageSetPointer>(XML_TYPE);

		mPointer = nullptr;
		mMousePointer = nullptr;
		mWidgetOwner = nullptr;
		mVisible = true;
		mDefaultPointer = "Default";
		mLayerName = "Pointer";

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void PointerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		FactoryManager::getInstance().unregistryFactory<ResourceManualPointer>(XML_TYPE);
		FactoryManager::getInstance().unregistryFactory<ResourceImageSetPointer>(XML_TYPE);

		// удаляем все виджеты
		_destroyAllChildWidget();

		clear();

		mWidgetOwner = nullptr;

		WidgetManager::getInstance().unregisterUnlinker(this);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool PointerManager::load(const std::string& _file)
	{
		return ResourceManager::getInstance()._loadImplement(_file, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void PointerManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		if (mMousePointer == nullptr)
		{
			mMousePointer = static_cast<StaticImagePtr>(baseCreateWidget(WidgetStyle::Overlapped, StaticImage::getClassTypeName(), "StaticImage", IntCoord(), Align::Default, "", ""));
			setLayerName(mLayerName);
		}

		if (_version < Version(1, 1))
		{
			loadObsoleteFormat(_node, _file, _version);
			return;
		}

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next(XML_TYPE))
		{
			std::string type = info->findAttribute("type");
			std::string name = info->findAttribute("name");

			IObject* object = FactoryManager::getInstance().createObject(XML_TYPE, type);
			if (object != nullptr)
			{
				IPointer* data = object->castType<IPointer>();
				data->deserialization(info.current(), _version);

				mResources[name] = data;
			}
		}

		// ставим дефолтный указатель
		setPointer(mDefaultPointer, nullptr);
	}

	void PointerManager::setVisible(bool _visible)
	{
		if (nullptr != mMousePointer) mMousePointer->setVisible(_visible);
		mVisible = _visible;
	}

	void PointerManager::setPosition(const IntPoint& _pos)
	{
		mPoint = _pos;
		if (nullptr != mMousePointer && mPointer != nullptr)
			mPointer->setPosition(mMousePointer, mPoint);
	}

	void PointerManager::setPointer(const std::string& _name, WidgetPtr _owner)
	{
		if (nullptr == mMousePointer) return;

		MapResource::iterator iter = mResources.find(_name);
		if (iter == mResources.end())
		{
			iter = mResources.find(mDefaultPointer);
			if (iter == mResources.end())
			{
				mPointer = nullptr;
				mMousePointer->setVisible(false);
				return;
			}
		}

		mMousePointer->setVisible(true);
		mPointer = iter->second;
		mPointer->setImage(mMousePointer);
		mPointer->setPosition(mMousePointer, mPoint);

		mWidgetOwner = _owner;
	}

	void PointerManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == mWidgetOwner) setPointer(mDefaultPointer, nullptr);
		else if (_widget == mMousePointer) mMousePointer = nullptr;
	}

	void PointerManager::setDefaultPointer()
	{
		if (!mDefaultPointer.empty())
			setPointer(mDefaultPointer, nullptr);
	}

	// создает виджет
	WidgetPtr PointerManager::baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, _align, nullptr, nullptr, this, _name);
		mWidgetChild.push_back(widget);
		// присоединяем виджет с уровню
		if (false == _layer.empty()) LayerManager::getInstance().attachToLayerNode(_layer, widget);
		return widget;
	}

	// удяляет неудачника
	void PointerManager::_destroyChildWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(nullptr != _widget, "invalid widget pointer");

		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if (iter != mWidgetChild.end())
		{
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
		WidgetManager& manager = WidgetManager::getInstance();
		while (false == mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			WidgetPtr widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			manager.unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			_deleteWidget(widget);
		}
	}

	void PointerManager::setDeafultPointer(const std::string& _value)
	{
		mDefaultPointer = _value;
		setPointer(mDefaultPointer, nullptr);
	}

	void PointerManager::setLayerName(const std::string& _value)
	{
		mLayerName = _value;
		LayerManager::getInstance().attachToLayerNode(mLayerName, mMousePointer);
	}

	void PointerManager::loadObsoleteFormat(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		std::string layer, def;

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator pointer = _node->getElementEnumerator();
		while (pointer.next(XML_TYPE))
		{
			// парсим атрибуты
			pointer->findAttribute("layer", layer);
			if (pointer->findAttribute("default", def)) mDefaultPointer = def;

			// сохраняем
			std::string text = pointer->findAttribute("texture");

			// берем детей и крутимся, основной цикл
			xml::ElementEnumerator info = pointer->getElementEnumerator();
			while (info.next("Info"))
			{
				if (!text.empty())
					info.current()->addAttribute("texture", text);

				std::string name = info->findAttribute("name");

				//вставляем под новый формат
				std::string size = info->findAttribute("size");
				if (!size.empty())
				{
					xml::ElementPtr node = info.current()->createChild("Property");
					node->addAttribute("key", "size");
					node->addAttribute("value", size);
				}

				std::string point = info->findAttribute("point");
				if (!point.empty())
				{
					xml::ElementPtr node = info.current()->createChild("Property");
					node->addAttribute("key", "point");
					node->addAttribute("value", point);
				}

				std::string texture = info->findAttribute("texture");
				if (!texture.empty())
				{
					xml::ElementPtr node = info.current()->createChild("Property");
					node->addAttribute("key", "texture");
					node->addAttribute("value", texture);
				}

				std::string offset = info->findAttribute("offset");
				if (!offset.empty())
				{
					xml::ElementPtr node = info.current()->createChild("Property");
					node->addAttribute("key", "coord");
					node->addAttribute("value", offset);
				}

				std::string resource = info->findAttribute("resource");
				if (!resource.empty())
				{
					xml::ElementPtr node = info.current()->createChild("Property");
					node->addAttribute("key", "resource");
					node->addAttribute("value", resource);
				}

				IObject* object = FactoryManager::getInstance().createObject(
					XML_TYPE,
					resource.empty() ? "ResourceManualPointer" : "ResourceImageSetPointer");
				if (object != nullptr)
				{
					IPointer* data = object->castType<IPointer>();
					data->deserialization(info.current(), _version);

					mResources[name] = data;
				}

			};
		};

		// если есть левел, то пересоеденяем, если нет виджета, то создаем
		if (!layer.empty())
			setLayerName(layer);

		// ставим дефолтный указатель
		setPointer(mDefaultPointer, nullptr);
	}

} // namespace MyGUI
