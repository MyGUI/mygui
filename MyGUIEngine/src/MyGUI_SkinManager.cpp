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
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_SkinInfo.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_IStateInfo.h"

namespace MyGUI
{

	const std::string XML_TYPE("Skin");

	MYGUI_INSTANCE_IMPLEMENT(SkinManager);

	void SkinManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &SkinManager::_load);
		FactoryManager::getInstance().registryFactory<SkinInfo>(XML_TYPE);

		createDefault();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SkinManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);
		FactoryManager::getInstance().unregistryFactory<SkinInfo>(XML_TYPE);

		clear();
		/*for (MapWidgetSkinInfoPtr::iterator iter=mSkins.begin(); iter!=mSkins.end(); ++iter)
		{
			SkinInfoPtr info = iter->second;
			info->clear();
			delete info;
		}
		mSkins.clear();*/

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool SkinManager::load(const std::string& _file/*, const std::string& _group*/)
	{
		return ResourceManager::getInstance()._loadImplement(_file, /*_group, */true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void SkinManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		// берем детей и крутимся, основной цикл со скинами
		xml::ElementEnumerator skin = _node->getElementEnumerator();
		while (skin.next(XML_TYPE))
		{
			std::string name = skin->findAttribute("name");
			std::string type = skin->findAttribute("type");
			if (type.empty()) type = "SkinInfo";

			IObject* object = FactoryManager::getInstance().createObject(XML_TYPE, type);
			if (object != nullptr)
			{
				SkinInfo* data = object->castType<SkinInfo>();
				data->deserialization(skin.current(), _version);

				if (remove(name))
				//if (mResources.find(name) != mResources.end())
				{
					MYGUI_LOG(Warning, "Skin with name '" + name + "' already exist");
					//mResources[name]->clear();
					//delete mResources[name];
				}
				mResources[name] = data;
			}
		}
	}

	/*SkinInfo * SkinManager::getSkin(const std::string& _name)
	{
		MapWidgetSkinInfoPtr::iterator iter = mSkins.find(_name);
		// если не нашли, то вернем дефолтный скин
		if (iter == mSkins.end())
		{
			MYGUI_LOG(Warning, "Skin '" << _name << "' not found, set Default");
			return mSkins["Default"];
		}
		return iter->second;
	}*/

	IntSize SkinManager::getTextureSize(const std::string& _texture)
	{
		// предыдущя текстура
		static std::string old_texture;
		static IntSize old_size;

		if (old_texture == _texture) return old_size;
		old_texture = _texture;
		old_size.clear();

		if (_texture.empty()) return old_size;

		RenderManager& render = RenderManager::getInstance();
		if (nullptr == render.getByName(_texture))
		{
			//const std::string& group = Gui::getInstance().getResourceGroup();
			DataManager& resourcer = DataManager::getInstance();
			if (!resourcer.isDataExist(_texture/*, group*/))
			{
				MYGUI_LOG(Error, "Texture '" + _texture + "' not found");// in group '" << group << "'");
				return old_size;
			}
			else
			{
				ITexture* texture = render.createTexture( _texture/* , group*/ );
				texture->loadFromFile(_texture);
			}
		}

		ITexture* texture = render.getByName(_texture);
		if (texture == nullptr)
		{
			MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
			return old_size;
		}

		old_size.set(texture->getWidth(), texture->getHeight());

#if MYGUI_DEBUG_MODE == 1
		if (isPowerOfTwo(old_size) == false)
		{
			MYGUI_LOG(Warning, "Texture '" + _texture + "' have non power of two size");
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

	bool SkinManager::isPowerOfTwo(const IntSize& _size)
	{
		int count = 0;
		IntSize size = _size;
		while (size.width > 0)
		{
			count += size.width & 1;
			size.width >>= 1;
		};
		if (count != 1) return false;
		count = 0;
		while (size.height > 0)
		{
			count += size.height & 1;
			size.height >>= 1;
		};
		if (count != 1) return false;
		return true;
	}

	void SkinManager::createDefault()
	{
		// создаем дефолтный скин
		const std::string name = "Default";
		/*IObject* object = FactoryManager::getInstance().createObject(XML_TYPE, type);
		if (object != nullptr)
		{
			SkinInfo* data = object->castType<SkinInfo>();
			mResources[_name] = skin;
		}*/
		SkinInfo* skin = new SkinInfo(name);
		mResources[name] = skin;
		//SkinInfo* widget_info = create("Default");
		//widget_info->setInfo(IntSize(0, 0), "");
	}

	//	для ручного создания скина
	/*SkinInfo* SkinManager::create(const std::string& _name)
	{
		SkinInfo * skin = new SkinInfo(_name);
		if (mSkins.find(_name) != mSkins.end())
		{
			MYGUI_LOG(Warning, "Skin with name '" + _name + "' already exist");
			mSkins[_name]->clear();
			delete mSkins[_name];
		}
		mSkins[_name] = skin;
		return skin;
	}*/

} // namespace MyGUI
