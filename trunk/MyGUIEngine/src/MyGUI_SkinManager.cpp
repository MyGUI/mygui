/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Gui.h"
#include "MyGUI_XmlDocument.h"

#include <OgreMaterialManager.h>

namespace MyGUI
{

	const std::string XML_TYPE("Skin");

	SkinManager::MapAlign SkinManager::mMapAlign;

	INSTANCE_IMPLEMENT(SkinManager);

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &SkinManager::_load);

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

		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		for (MapWidgetSkinInfoPtr::iterator iter=mSkins.begin(); iter!=mSkins.end(); ++iter) {
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
		const std::vector<std::string> & vec = utility::split(_value);
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
			MYGUI_LOG(Warning, "no find skin '" << _name << "', set Default");
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

	bool SkinManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void SkinManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// вспомогательный класс для биндинга сабскинов
		SubWidgetBinding bind;

		// берем детей и крутимся, основной цикл со скинами
		xml::xmlNodeIterator skin = _node->getNodeIterator();
		while (skin.nextNode(XML_TYPE)) {

			// парсим атрибуты скина
			Ogre::String name, texture, tmp;
			IntSize size;
			skin->findAttribute("name", name);
			skin->findAttribute("texture", texture);
			if (skin->findAttribute("size", tmp)) size = IntSize::parse(tmp);

			// создаем скин
			WidgetSkinInfo * widget_info = create(name);
			widget_info->setInfo(size, texture);
			FloatSize materialSize = getTextureSize(texture);

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
						if (state->findAttribute("offset", tmp)) offset = convertTextureCoord(FloatRect::parse(tmp), materialSize);
						if (state->findAttribute("colour", tmp)) colour = utility::parseColour(tmp);
						if (state->findAttribute("alpha", tmp)) alpha = utility::parseFloat(tmp);

						// добавляем инфо о стайте
						bind.add(basisStateName, offset, colour, alpha);

					};

					// теперь всё вместе добавляем в скин
					widget_info->addInfo(bind);
				}

			};
		};
	}	

	FloatSize SkinManager::getTextureSize(const std::string & _texture)
	{
		FloatSize size(1, 1);

		if (_texture.empty()) return size;

		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		if (false == manager.resourceExists(_texture)) 
			manager.load(_texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		Ogre::TexturePtr tex = (Ogre::TexturePtr)manager.getByName(_texture);
		if (tex.isNull()) return size;

		size.width = (float)tex->getWidth();
		size.height = (float)tex->getHeight();

		if (size.width < 1) size.width = 1;
		if (size.height < 1) size.height = 1;

		return size;
	}

	FloatRect SkinManager::convertTextureCoord(const FloatRect & _source, const FloatSize & _textureSize)
	{
		FloatRect retRect;
		if (!_textureSize.width || !_textureSize.height) return retRect;

		retRect.left = _source.left / _textureSize.width;
		retRect.top = _source.top / _textureSize.height;
		retRect.right = (_source.left + _source.right) / _textureSize.width;
		retRect.bottom = (_source.top + _source.bottom) / _textureSize.height;

		return retRect;
	}

	void SkinManager::createDefault()
	{
		// создаем дефолтный скин
		WidgetSkinInfo * widget_info = create("Default");
		widget_info->setInfo(IntSize(0, 0), "");
	}


} // namespace MyGUI
