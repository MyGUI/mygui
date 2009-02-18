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
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	const std::string XML_TYPE("Skin");

	MYGUI_INSTANCE_IMPLEMENT(SkinManager);

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &SkinManager::_load);

		createDefault();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SkinManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		for (MapWidgetSkinInfoPtr::iterator iter=mSkins.begin(); iter!=mSkins.end(); ++iter) {
			WidgetSkinInfoPtr info = iter->second;
			info->clear();
			delete info;
		}
		mSkins.clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	WidgetSkinInfo * SkinManager::getSkin(const Ogre::String & _name)
	{
		MapWidgetSkinInfoPtr::iterator iter = mSkins.find(_name);
		// если не нашли, то вернем дефолтный скин
		if (iter == mSkins.end()) {
			MYGUI_LOG(Warning, "Skin '" << _name << "' not found, set Default");
			return mSkins["Default"];
		}
		return iter->second;
	}

	//	для ручного создания скина
	WidgetSkinInfo * SkinManager::create(const Ogre::String & _name)
	{
		WidgetSkinInfo * skin = new WidgetSkinInfo();
		if (mSkins.find(_name) != mSkins.end()){
			MYGUI_LOG(Warning, "Skin with name '" + _name + "' already exist");
			mSkins[_name]->clear();
			delete mSkins[_name];
		}
		mSkins[_name] = skin;
		return skin;
	}

	bool SkinManager::load(const std::string & _file, const std::string & _group)
	{
		return ResourceManager::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void SkinManager::_load(xml::ElementPtr _node, const std::string & _file, Version _version)
	{
		LanguageManager& localizator = LanguageManager::getInstance();

		// вспомогательный класс для биндинга сабскинов
		SubWidgetBinding bind;

		// берем детей и крутимся, основной цикл со скинами
		xml::ElementEnumerator skin = _node->getElementEnumerator();
		while (skin.next(XML_TYPE)) {

			// парсим атрибуты скина
			Ogre::String name, texture, tmp;
			IntSize size;
			skin->findAttribute("name", name);
			skin->findAttribute("texture", texture);
			if (skin->findAttribute("size", tmp)) size = IntSize::parse(tmp);

			// поддержка замены тегов в скинах
			if (_version >= Version(1, 1)) {
				texture = localizator.replaceTags(texture);
			}

			// создаем скин
			WidgetSkinInfo * widget_info = create(name);
			widget_info->setInfo(size, texture);
			IntSize materialSize = getTextureSize(texture);

			// проверяем маску
			if (skin->findAttribute("mask", tmp)) {
				if (false == widget_info->loadMask(tmp)) {
					MYGUI_LOG(Error, "Skin: " << _file << ", mask not load '" << tmp << "'");
				}
			}

			// берем детей и крутимся, цикл с саб скинами
			xml::ElementEnumerator basis = skin->getElementEnumerator();
			while (basis.next()) {

				if (basis->getName() == "Property") {
					// загружаем свойства
					std::string key, value;
					if (false == basis->findAttribute("key", key)) continue;
					if (false == basis->findAttribute("value", value)) continue;

					// поддержка замены тегов в скинах
					if (_version >= Version(1, 1)) {
						value = localizator.replaceTags(value);
					}

					// добавляем свойство
					widget_info->addProperty(key, value);

				}
				else if (basis->getName() == "Child") {
					ChildSkinInfo child(
						basis->findAttribute("type"),
						WidgetStyle::parse(basis->findAttribute("style")),
						basis->findAttribute("skin"),
						IntCoord::parse(basis->findAttribute("offset")),
						Align::parse(basis->findAttribute("align")),
						basis->findAttribute("layer"),
						basis->findAttribute("name")
						);

					xml::ElementEnumerator child_params = basis->getElementEnumerator();
					while (child_params.next("Property"))
						child.addParam(child_params->findAttribute("key"), child_params->findAttribute("value"));

					widget_info->addChild(child);
					//continue;

				}
				else if (basis->getName() == "BasisSkin") {
					// парсим атрибуты
					Ogre::String basisSkinType, tmp;
					IntCoord offset;
					Align align = Align::Default;
					basis->findAttribute("type", basisSkinType);
					if (basis->findAttribute("offset", tmp)) offset = IntCoord::parse(tmp);
					if (basis->findAttribute("align", tmp)) align = Align::parse(tmp);

					bind.create(offset, align, basisSkinType);

					// берем детей и крутимся, цикл со стейтами
					xml::ElementEnumerator state = basis->getElementEnumerator();

					// проверяем на новый формат стейтов
					bool new_format = false;
					// если версия меньше 1.0 то переименовываем стейты
					if (_version < Version(1, 0)) {
						while (state.next()) {
							if (state->getName() == "State") {
								const std::string & name_state = state->findAttribute("name");
								if ((name_state == "normal_checked") || (state->findAttribute("name") == "normal_check")) {
									new_format = true;
									break;
								}
							}
						};
						// обновляем
						state = basis->getElementEnumerator();
					}

					while (state.next()) {

						if (state->getName() == "State") {
							// парсим атрибуты стейта
							Ogre::String basisStateName;
							state->findAttribute("name", basisStateName);

							// если версия меньше 1.0 то переименовываем стейты
							if (_version < Version(1, 0)) {
								// это обсолет новых типов
								if (basisStateName == "disable_check") basisStateName = "disabled_checked";
								else if (basisStateName == "normal_check") basisStateName = "normal_checked";
								else if (basisStateName == "active_check") basisStateName = "highlighted_checked";
								else if (basisStateName == "pressed_check") basisStateName = "pushed_checked";
								else if (basisStateName == "disable") basisStateName = "disabled";
								else if (basisStateName == "active") basisStateName = "highlighted";
								else if (basisStateName == "select") basisStateName = "pushed";
								else if (basisStateName == "pressed") {
									if (new_format) basisStateName = "pushed";
									else basisStateName = "normal_checked";
								}
							}

							// конвертируем инфу о стейте
							StateInfo * data = SubWidgetManager::getInstance().getStateData(basisSkinType, state.current(), skin.current(), _version);

							// добавляем инфо о стайте
							bind.add(basisStateName, data, name);

						}
						else if (state->getName() == "Property") {
							// загружаем свойства
							std::string key, value;
							if (false == state->findAttribute("key", key)) continue;
							if (false == state->findAttribute("value", value)) continue;

							// поддержка замены тегов в скинах
							/*if (_version >= Version(1, 1)) {
								value = localizator.replaceTags(value);
							}*/
							// добавляем свойство
							bind.addProperty(key, value);
						}

					};

					// теперь всё вместе добавляем в скин
					widget_info->addInfo(bind);
				}

			};
		};
	}

	IntSize SkinManager::getTextureSize(const std::string & _texture)
	{
		// предыдущя текстура
		static std::string old_texture;
		static IntSize old_size;

		if (old_texture == _texture) return old_size;
		old_texture = _texture;
		old_size.clear();

		if (_texture.empty()) return old_size;

		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		if (false == manager.resourceExists(_texture)) {

			std::string group = Gui::getInstance().getResourceGroup();

			if (!helper::isFileExist(_texture, group)) {
				MYGUI_LOG(Error, "Texture '" + _texture + "' not found in group '" << group << "'");
				return old_size;
			}
			else {
				manager.load(
					_texture,
					group,
					Ogre::TEX_TYPE_2D,
					0);
			}
		}

		Ogre::TexturePtr tex = (Ogre::TexturePtr)manager.getByName(_texture);
		if (tex.isNull()) {
			MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
			return old_size;
		}
		tex->load();

		old_size.set((int)tex->getWidth(), (int)tex->getHeight());

#if MYGUI_DEBUG_MODE == 1
		if (isPowerOfTwo(old_size) == false) {
			MYGUI_LOG(Warning, "Texture '" + _texture + "' have non power ow two size");
		}
#endif

		return old_size;
	}

	FloatRect SkinManager::convertTextureCoord(const FloatRect & _source, const IntSize & _textureSize)
	{
		if (!_textureSize.width || !_textureSize.height) return FloatRect();
		return FloatRect(
			_source.left / _textureSize.width,
			_source.top / _textureSize.height,
			(_source.left + _source.right) / _textureSize.width,
			(_source.top + _source.bottom) / _textureSize.height);
	}

	void SkinManager::createDefault()
	{
		// создаем дефолтный скин
		WidgetSkinInfo * widget_info = create("Default");
		widget_info->setInfo(IntSize(0, 0), "");
	}

	bool SkinManager::isPowerOfTwo(IntSize _size)
	{
		int count = 0;
		while (_size.width > 0) {
			count += _size.width & 1;
			_size.width >>= 1;
		};
		if (count != 1) return false;
		count = 0;
		while (_size.height > 0) {
			count += _size.height & 1;
			_size.height >>= 1;
		};
		if (count != 1) return false;
		return true;
	}

} // namespace MyGUI
