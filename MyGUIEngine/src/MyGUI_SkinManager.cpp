/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "xmlDocument.h"

#include <OgreMaterialManager.h>

namespace MyGUI
{

	INSTANCE_IMPLEMENT(SkinManager);

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, "initialise already");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// забиваем карту флагами выравнивания
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_HCENTER);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_VCENTER);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_CENTER);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_CENTER_PARENT);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_LEFT);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_RIGHT);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_HSTRETCH);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_TOP);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_BOTTOM);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_VSTRETCH);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_STRETCH);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_DEFAULT);

		createDefault();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SkinManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		for (MapWidgetSkinInfoPtr::iterator iter=mSkins.begin(); iter!=mSkins.end(); iter++) {
			WidgetSkinInfoPtr info = iter->second;
			delete info;
		}
		mSkins.clear();
		mMapAlign.clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	Align SkinManager::parseAlign(const std::string & _value)
	{
		Align flag = 0;
		const std::vector<std::string> & vec = util::split(_value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			MapAlign::iterator iter = mMapAlign.find(vec[pos]);
			if (iter != mMapAlign.end()) flag |= iter->second;
			else {
				MYGUI_LOG(Warning, "Cannot parse align '" << vec[pos] << "'");
			}
		}
		return flag;
	}

	WidgetSkinInfo * SkinManager::getSkin(const Ogre::String & _name)
	{
		MapWidgetSkinInfoPtr::iterator iter = mSkins.find(_name);
		// если не нашли, то вернем дефолтный скин
		if (iter == mSkins.end()) {
			MYGUI_LOG(Warning, "no find skin, set default");
			return mSkins["Default"];
		}
		return iter->second;
	}

	//	для ручного создания скина
	WidgetSkinInfo * SkinManager::create(const Ogre::String & _name)
	{
		WidgetSkinInfo * skin = new WidgetSkinInfo();
		mSkins[_name] = skin;
		return skin;
	}

	bool SkinManager::load(const std::string & _file, bool _resource)
	{
		xml::xmlDocument doc;
		std::string file = (_resource ? helper::getResourcePath(_file) : _file).c_str();
		if ("" == file) {
			MYGUI_LOG(Error, "Skin: " << _file << " not found");
			return false;
		}
		if (false == doc.open(file)) {
			MYGUI_LOG(Error, "Skin: " << doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (root == 0) || (root->getName() != "MyGUI") ) {
			MYGUI_LOG(Error, "Skin: " << _file << " root tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if ( (false == root->findAttribute("type", type)) || (type != "Skin") ) {
			MYGUI_LOG(Error, "Skin: " << _file << " root type 'Skin' not found");
			return false;
		}

		// вспомогательный класс для биндинга сабскинов
		SubWidgetBinding bind;

		// берем детей и крутимся, основной цикл со скинами
		xml::xmlNodeIterator skin = root->getNodeIterator();
		while (skin.nextNode("Skin")) {

			// парсим атрибуты скина
			Ogre::String skinName, skinMaterial, tmp;
			IntSize size;
			skin->findAttribute("name", skinName);
			skin->findAttribute("material", skinMaterial);
			if (skin->findAttribute("size", tmp)) size = IntSize::parse(tmp);

			// создаем скин
			WidgetSkinInfo * widget_info = create(skinName);
			widget_info->setInfo(size, skinMaterial);
			FloatSize materialSize = getMaterialSize(skinMaterial);

			// проверяем маску
			if (skin->findAttribute("mask", tmp)) {
				if (false == widget_info->loadMask(tmp)) {
					MYGUI_LOG(Error, "Skin: " << _file << ", mask not load '" << tmp << "'");
				}
			}

			// берем детей и крутимся, цикл с саб скинами
			xml::xmlNodeIterator basis = skin->getNodeIterator();
			while (basis.nextNode()) {

				if (basis->getName() == "Property") {
					// загружаем свойства
					std::string key, value;
					if (false == basis->findAttribute("key", key)) continue;
					if (false == basis->findAttribute("value", value)) continue;
					// добавляем свойство
					widget_info->addParam(key, value);

				}
				else if (basis->getName() == "Child") {
					ChildSkinInfo child(
						basis->findAttribute("type"),
						basis->findAttribute("skin"),
						basis->findAttribute("name"),
						IntCoord::parse(basis->findAttribute("offset")),
						parseAlign(basis->findAttribute("align")),
						basis->findAttribute("layer")
						);

					xml::xmlNodeIterator child_params = basis->getNodeIterator();
					while (child_params.nextNode("Property"))
						child.addParam(child_params->findAttribute("key"), child_params->findAttribute("value"));

					widget_info->addChild(child);
					//continue;

				}
				else if (basis->getName() == "BasisSkin") {
					// парсим атрибуты
					Ogre::String basisSkinType, tmp;
					IntCoord offset;
					Align align = ALIGN_DEFAULT;
					basis->findAttribute("type", basisSkinType);
					if (basis->findAttribute("offset", tmp)) offset = IntCoord::parse(tmp);
					if (basis->findAttribute("align", tmp)) align = parseAlign(tmp);

					bind.create(offset, align, basisSkinType);

					// берем детей и крутимся, цикл со стейтами
					xml::xmlNodeIterator state = basis->getNodeIterator();
					while (state.nextNode("State")) {

						// парсим атрибуты стейта
						Ogre::String basisStateName, tmp;
						FloatRect offset;
						Ogre::ColourValue colour = Ogre::ColourValue::ZERO;
						float alpha = -1;

						state->findAttribute("name", basisStateName);
						if (state->findAttribute("offset", tmp)) offset = convertMaterialCoord(FloatRect::parse(tmp), materialSize);
						if (state->findAttribute("colour", tmp)) colour = util::parseColour(tmp);
						if (state->findAttribute("alpha", tmp)) alpha = util::parseFloat(tmp);

						// добавляем инфо о стайте
						bind.add(basisStateName, offset, colour, alpha);

					};

					// теперь всё вместе добавляем в скин
					widget_info->addInfo(bind);
				}

			};
		};
		return true;
	}

	FloatSize SkinManager::getMaterialSize(const std::string & _material)
	{
		FloatSize size(1, 1);

		if (_material.empty()) return size;

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(_material);
		if (mat.isNull()) return size;

		// обязательно загружаем
		mat->load();

		// только так, иначе при пустых викидывает
		Ogre::Material::TechniqueIterator iTechnique = mat->getTechniqueIterator();
		if ( ! iTechnique.hasMoreElements() ) return size;

		Ogre::Pass * pass = iTechnique.getNext()->getPass(0);
		if (!pass) return size;

		Ogre::Pass::TextureUnitStateIterator iUnit = pass->getTextureUnitStateIterator();
		if ( ! iUnit.hasMoreElements()) return size;

		const Ogre::String & textName = iUnit.getNext()->getTextureName();

		Ogre::TexturePtr tex = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(textName);
		if (tex.isNull()) return size;

		size.width = (float)tex->getWidth();
		size.height = (float)tex->getHeight();

		if (size.width < 1) size.width = 1;
		if (size.height < 1) size.height = 1;

		return size;
	}

	FloatRect SkinManager::convertMaterialCoord(const FloatRect & _source, const FloatSize & _materialSize)
	{
		FloatRect retRect;
		if (!_materialSize.width || !_materialSize.height) return retRect;

		retRect.left = _source.left / _materialSize.width;
		retRect.top = _source.top / _materialSize.height;
		retRect.right = (_source.left + _source.right) / _materialSize.width;
		retRect.bottom = (_source.top + _source.bottom) / _materialSize.height;

		return retRect;
	}

	void SkinManager::createDefault()
	{
		// создаем дефолтный скин
		WidgetSkinInfo * widget_info = create("Default");
		widget_info->setInfo(IntSize(0, 0), "DefaultSettings");
		SubWidgetBinding bind(IntCoord(0, 0, 1, 1), ALIGN_DEFAULT, "MainSkin");
		widget_info->addInfo(bind);
		// создаем дефолтный прозрачный скин
		widget_info = create("Empty");
		widget_info->setInfo(IntSize(0, 0), "");
		bind.create(IntCoord(0, 0, 1, 1), ALIGN_DEFAULT, "MainSkin");
		widget_info->addInfo(bind);
	}


} // namespace MyGUI
