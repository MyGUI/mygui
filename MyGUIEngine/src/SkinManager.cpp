
#include "SkinManager.h"
#include "WidgetSkinInfo.h"
#include "xmlDocument.h"

#include <OgreMaterialManager.h>

namespace MyGUI
{

	INSTANCE_IMPLEMENT(SkinManager);

	void SkinManager::initialise()
	{
		assert(!mIsInitialise);

		// забиваем карту флагами выравнивания
		REGISTER_VALUE(m_mapAlign, ALIGN_NONE);
		REGISTER_VALUE(m_mapAlign, ALIGN_HCENTER);
		REGISTER_VALUE(m_mapAlign, ALIGN_VCENTER);
		REGISTER_VALUE(m_mapAlign, ALIGN_CENTER);
		REGISTER_VALUE(m_mapAlign, ALIGN_CENTER_PARENT);
		REGISTER_VALUE(m_mapAlign, ALIGN_LEFT);
		REGISTER_VALUE(m_mapAlign, ALIGN_RIGHT);
		REGISTER_VALUE(m_mapAlign, ALIGN_HSTRETCH);
		REGISTER_VALUE(m_mapAlign, ALIGN_TOP);
		REGISTER_VALUE(m_mapAlign, ALIGN_BOTTOM);
		REGISTER_VALUE(m_mapAlign, ALIGN_VSTRETCH);
		REGISTER_VALUE(m_mapAlign, ALIGN_STRETCH);

		createDefault();

		mIsInitialise = true;
	}

	void SkinManager::shutdown()
	{
		if (!mIsInitialise) return;

		for (MapWidgetSkinInfoPtr::iterator iter=m_skins.begin(); iter!=m_skins.end(); iter++) {
			WidgetSkinInfoPtr info = iter->second;
			delete info;
		}
		m_skins.clear();

		mIsInitialise = false;
	}

	Align SkinManager::parseAlign(const std::string & _value)
	{
		Align flag = 0;
		const std::vector<std::string> & vec = util::split(_value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			MapFlags::iterator iter = m_mapAlign.find(vec[pos]);
			if (iter != m_mapAlign.end()) flag |= iter->second;
		}
		return flag;
	}

	WidgetSkinInfo * SkinManager::getSkin(const Ogre::String & _name)
	{
		MapWidgetSkinInfoPtr::iterator iter = m_skins.find(_name);
		// если не нашли, то вернем дефолтный скин
		if (iter == m_skins.end()) {
			LOG("no find skin, set default");
			return m_skins["Default"];
		}
		return iter->second;
	}

	//	для ручного создания скина
	WidgetSkinInfo * SkinManager::create(const Ogre::String & _name)
	{
		WidgetSkinInfo * skin = new WidgetSkinInfo();
		m_skins[_name] = skin;
		return skin;
	}

	void SkinManager::load(const std::string & _file)
	{
		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if ( (xml_root == 0) || (xml_root->getName() != "MyGUI_SkinInfo") ) return;

		// вспомогательный класс для биндинга сабскинов
		BasisWidgetBinding bind;

		// берем детей и крутимся, основной цикл со скинами
		xml::xmlNodeIterator skin = xml_root->getNodeIterator();
		while (skin.nextNode("Skin")) {

			// парсим атрибуты скина
			Ogre::String skinName, skinMaterial, tmp;
			IntSize size;
			skin->findAttribute("Name", skinName);
			skin->findAttribute("Material", skinMaterial);
			if (skin->findAttribute("Size", tmp)) size = util::parseIntSize(tmp);

			// создаем скин
			WidgetSkinInfo * widget_info = create(skinName);
			widget_info->setInfo(size, skinMaterial);
			FloatSize materialSize = getMaterialSize(skinMaterial);

			// берем детей и крутимся, цикл с саб скинами
			xml::xmlNodeIterator basis = skin->getNodeIterator();
			while (basis.nextNode()) {

				if (basis->getName() == "Property") {
					// загружаем свойства
					std::string key, value;
					if (false == basis->findAttribute("Key", key)) continue;
					if (false == basis->findAttribute("Value", value)) continue;
					// добавляем свойство
					widget_info->addParam(key, value);
					// все уходим
					continue;

				}
				else if (basis->getName() != "BasisSkin") continue;

				// парсим атрибуты
				Ogre::String basisSkinType, tmp;
				IntRect offset;
				Align align = ALIGN_NONE;
				basis->findAttribute("Type", basisSkinType);
				if (basis->findAttribute("Offset", tmp)) offset = util::parseIntRect(tmp);
				if (basis->findAttribute("Align", tmp)) align = parseAlign(tmp);

				bind.create(offset, align, basisSkinType);

				// берем детей и крутимся, цикл со стейтами
				xml::xmlNodeIterator state = basis->getNodeIterator();
				while (state.nextNode("State")) {

					// парсим атрибуты стейта
					Ogre::String basisStateName, tmp;
					FloatRect offset;
					Ogre::ColourValue colour = Ogre::ColourValue::ZERO;
					float alpha = -1;

					state->findAttribute("Name", basisStateName);
					if (state->findAttribute("Offset", tmp)) offset = convertMaterialCoord(util::parseFloatRect(tmp), materialSize);
					if (state->findAttribute("Color", tmp)) colour = util::parseColour(tmp);
					if (state->findAttribute("Alpha", tmp)) alpha = util::parseFloat(tmp);

					// добавляем инфо о стайте
					bind.add(basisStateName, offset, colour, alpha);

				};

				// теперь всё вместе добавляем в скин
				widget_info->addInfo(bind);

			};
		};
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
		widget_info->setInfo(IntSize(0, 0), Ogre::MaterialManager::getSingleton().getDefaultSettings()->getName());
		BasisWidgetBinding bind(IntRect(0, 0, 1, 1), ALIGN_NONE, "MainSkin");
		widget_info->addInfo(bind);
		// создаем дефолтный прозрачный скин
		widget_info = create("Empty");
		widget_info->setInfo(IntSize(0, 0), "");
		bind.create(IntRect(0, 0, 1, 1), ALIGN_NONE, "MainSkin");
		widget_info->addInfo(bind);
	}


} // namespace MyGUI