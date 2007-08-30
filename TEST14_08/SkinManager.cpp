
#include "SkinManager.h"
#include "WidgetSkinInfo.h"
#include "xmlDocument.h"

namespace widget
{

	SkinManager::SkinManager()
	{
		initialise();
		createDefault();
	}

	SkinManager::~SkinManager()
	{
		for (SkinInfo::iterator iter=m_skins.begin(); iter!=m_skins.end(); iter++) {
			WidgetSkinInfoPtr info = iter->second;
			delete info;
		}
		m_skins.clear();
	}

	char SkinManager::parseAlign(const std::string & _value)
	{
		char flag = 0;
		const std::vector<std::string> & vec = util::split(_value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			MapFlags::iterator iter = m_mapAlign.find(vec[pos]);
			if (iter != m_mapAlign.end()) flag |= iter->second;
		}
		return flag;
	}

	WidgetSkinInfo * SkinManager::getSkin(const Ogre::String & _name)
	{
		SkinInfo::iterator iter = m_skins.find(_name);
		// если не нашли, то вернем дефолтный скин
		if (iter == m_skins.end()) return m_skins["Default"];
		return iter->second;
	}

	//	для ручного создания скина
	WidgetSkinInfo * SkinManager::create(const Ogre::String & _name)
	{
		WidgetSkinInfo * skin = new WidgetSkinInfo();
		m_skins[_name] = skin;
		return skin;
	}

	void SkinManager::loadSkin(const std::string & _file)
	{
		xml::xmlDocument doc;
		if (!doc.load(_file)) OGRE_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if (xml_root == 0) return;
		if (xml_root->getName() != "MyGUI_SkinInfo") return;

		// вспомогательный класс для биндинга сабскинов
		BasisWidgetBinding bind;

		// берем детей и крутимся, основной цикл со скинами
		xml::VectorNode & skins = xml_root->getChilds();
		for (size_t i_skin=0; i_skin<skins.size(); i_skin++) {
			xml::xmlNodePtr skinInfo = skins[i_skin];
			if (skinInfo->getName() != "Skin") continue;

			// парсим атрибуты скина
			const xml::VectorAttributes & attrib = skinInfo->getAttributes();
			Ogre::String skinName, skinMaterial;//, fontName;
			intSize size;
			for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
				// достаем пару атрибут - значение
				const xml::PairAttributes & pairAttributes = attrib[i_attrib];
				if (pairAttributes.first == "Name") skinName = pairAttributes.second;
				else if (pairAttributes.first == "Material") skinMaterial = pairAttributes.second;
				else if (pairAttributes.first == "Size") size = intSize::parse(pairAttributes.second);
			}

			// создаем скин
			WidgetSkinInfo * widget_info = create(skinName);
			widget_info->setInfo(size, skinMaterial);
			floatSize materialSize = getMaterialSize(skinMaterial);

			// берем детей и крутимся, цикл с саб скинами
			xml::VectorNode & basisSkins = skinInfo->getChilds();
			for (size_t i_basis=0; i_basis<basisSkins.size(); i_basis++) {
				xml::xmlNodePtr basisSkinInfo = basisSkins[i_basis];

				if (basisSkinInfo->getName() == "Property") {
					// загружаем свойства
					const xml::VectorAttributes & attrib = basisSkinInfo->getAttributes();
					std::string key, value;
					for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
						// достаем пару атрибут - значение
						const xml::PairAttributes & pairAttributes = attrib[i_attrib];
						if (pairAttributes.first == "Key") key = pairAttributes.second;
						else if (pairAttributes.first == "Value") value = pairAttributes.second;
					}
					// добавляем свойство
					widget_info->addParam(key, value);
					// все уходим
					continue;

				} else if (basisSkinInfo->getName() != "BasisSkin") continue;

				// парсим атрибуты саб скина
				const xml::VectorAttributes & attrib = basisSkinInfo->getAttributes();
				Ogre::String basisSkinType;
				intRect offset;
				char align = ALIGN_NONE;
				for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
					// достаем пару атрибут - значение
					const xml::PairAttributes & pairAttributes = attrib[i_attrib];
					if (pairAttributes.first == "Type") basisSkinType = pairAttributes.second;
					else if (pairAttributes.first == "Offset") offset = intRect::parse(pairAttributes.second);
					else if (pairAttributes.first == "Align") align = parseAlign(pairAttributes.second);
				}

				bind.create(offset, align, basisSkinType);

				// берем детей и крутимся, цикл со стейтами
				xml::VectorNode & basisState = basisSkinInfo->getChilds();
				for (size_t i_state=0; i_state<basisState.size(); i_state++) {
					xml::xmlNodePtr basisStateInfo = basisState[i_state];
					if (basisStateInfo->getName() != "State") continue;

					// парсим атрибуты стейта
					const xml::VectorAttributes & attrib = basisStateInfo->getAttributes();
					Ogre::String basisStateName;
					floatRect offset;
					for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
						// достаем пару атрибут - значение
						const xml::PairAttributes & pairAttributes = attrib[i_attrib];
						if (pairAttributes.first == "Name") basisStateName = pairAttributes.second;
						else if (pairAttributes.first == "Offset") offset = convertMaterialCoord(floatRect::parse(pairAttributes.second), materialSize);
					}
					// добавляем инфо о стайте
					bind.add(basisStateName, offset);

				} // for (size_t i_state=0; i_state<basisState.size(); i_state++) {

				// теперь всё вместе добавляем в скин
				widget_info->addInfo(bind);

			} // for (size_t i_basis=0; i_basis<basisSkins.size(); i_basis++) {
		} // for (size_t i_skin=0; i_skin<skins.size(); i_skin++) {
	}

	floatSize SkinManager::getMaterialSize(const std::string & _material)
	{
		floatSize size(1, 1);

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

		return size;
	}

/*	bool SkinManager::getMaterialSize(float & _cx, float & _cy, const std::string & _material)
	{
		_cx = 1;
		_cy = 1;

		if (_material.empty()) return false;

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(_material);
		if (mat.isNull()) return false;

		// обязательно загружаем
		mat->load();

		// только так, иначе при пустых викидывает
		Ogre::Material::TechniqueIterator iTechnique = mat->getTechniqueIterator();
		if ( ! iTechnique.hasMoreElements() ) return false;

		Ogre::Pass * pass = iTechnique.getNext()->getPass(0);
		if (!pass) return false;

		Ogre::Pass::TextureUnitStateIterator iUnit = pass->getTextureUnitStateIterator();
		if ( ! iUnit.hasMoreElements()) return false;

		const Ogre::String & textName = iUnit.getNext()->getTextureName();

		Ogre::TexturePtr tex = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(textName);
		if (tex.isNull()) return false;

		_cx = (float)tex->getWidth();
		_cy = (float)tex->getHeight();

		return true;
	}*/

/*	floatRect SkinManager::convertMaterialCoord(const floatRect & _source, float _cx, float _cy)
	{
		floatRect retRect;
		if (!_cx || !_cy) return retRect;

		retRect.left = _source.left / _cx;
		retRect.top = _source.top / _cy;
		retRect.right = (_source.left + _source.right) / _cx;
		retRect.bottom = (_source.top + _source.bottom) / _cy;

		return retRect;
	}*/

	floatRect SkinManager::convertMaterialCoord(const floatRect & _source, const floatSize & _materialSize)
	{
		floatRect retRect;
		if (!_materialSize.width || !_materialSize.height) return retRect;

		retRect.left = _source.left / _materialSize.width;
		retRect.top = _source.top / _materialSize.height;
		retRect.right = (_source.left + _source.right) / _materialSize.width;
		retRect.bottom = (_source.top + _source.bottom) / _materialSize.height;

		return retRect;
	}

	void SkinManager::initialise()
	{
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
	}
	
	void SkinManager::createDefault()
	{
		// создаем дефолтный скин
		WidgetSkinInfo * widget_info = create("Default");
		widget_info->setInfo(intSize(0, 0), Ogre::MaterialManager::getSingleton().getDefaultSettings()->getName());
		BasisWidgetBinding bind(0, 0, 1, 1, ALIGN_NONE, "MainSkin");
		widget_info->addInfo(bind);
	}


} // namespace widget