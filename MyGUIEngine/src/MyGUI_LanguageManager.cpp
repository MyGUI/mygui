/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
#include "MyGUI_LanguageManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_ResourceLanguage.h"
#include "MyGUI_FactoryManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("Language");
	const std::string XML_TYPE_TAG("Tag");
	const std::string XML_TYPE_RESOURCE("Resource");
	const std::string XML_TYPE_PROPERTY("Property");
	const std::string XML_TYPE_SOURCE("Source");

	MYGUI_INSTANCE_IMPLEMENT(LanguageManager);

	void LanguageManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LanguageManager::_load);
		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE_TAG) = newDelegate(this, &LanguageManager::_loadSource);
		FactoryManager::getInstance().registryFactory<ResourceLanguage>(XML_TYPE_RESOURCE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LanguageManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE_TAG);
		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);
		FactoryManager::getInstance().unregistryFactory<ResourceLanguage>(XML_TYPE_RESOURCE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool LanguageManager::load(const std::string& _file)
	{
		return ResourceManager::getInstance()._loadImplement(_file, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LanguageManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		std::string default_language;

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next())
		{
			if (root->getName() == XML_TYPE)
			{
				// парсим атрибуты
				std::string def = root->findAttribute("default");
				if (!def.empty())
					default_language = def;

				// берем детей и крутимся
				xml::ElementEnumerator info = root->getElementEnumerator();
				while (info.next("Info"))
				{
					// парсим атрибуты
					std::string name(info->findAttribute("name"));
					std::string type = ResourceLanguage::getClassTypeName();

					IObject* object = FactoryManager::getInstance().createObject(XML_TYPE_RESOURCE, type);
					if (object != nullptr)
					{
						ResourceLanguage* data = object->castType<ResourceLanguage>();
						data->deserialization(info.current(), _version);

						ResourceManager::getInstance().addResource(data);

						// пользовательские теги
						if (name.empty())
						{
							Enumerator<VectorString> tag = data->getEnumerator();
							while (tag.next())
							{
								loadLanguage(tag.current(), true);
							}
						}
					}

				}
			}
			else if (root->getName() == XML_TYPE_PROPERTY)
			{
				const std::string& key = root->findAttribute("key");
				const std::string& value = root->findAttribute("value");
				if (key == "Default")
					setCurrentLanguage(value);
			}
		}

		if (!default_language.empty())
			setCurrentLanguage(default_language);
	}

	void LanguageManager::setCurrentLanguage(const std::string& _name)
	{
		loadResourceLanguage(_name);
		mCurrentLanguageName = _name;
		eventChangeLanguage(mCurrentLanguageName);
	}

	bool LanguageManager::loadResourceLanguage(const std::string& _name)
	{
		mMapLanguage.clear();

		IResource* data = ResourceManager::getInstance().getByName(_name, false);
		if (data == nullptr)
			return false;

		ResourceLanguage* lang = data->castType<ResourceLanguage>(false);
		if (lang == nullptr)
			return false;

		Enumerator<VectorString> source = lang->getEnumerator();
		while (source.next())
		{
			loadLanguage(source.current());
		}

		return true;
	}

	bool LanguageManager::loadLanguage(const std::string& _file, bool _user)
	{
		IDataStream* data = DataManager::getInstance().getData(_file);
		if (data == nullptr)
		{
			MYGUI_LOG(Error, "file '" << _file << "' not found");
			return false;
		}

		if (_file.find(".xml") != std::string::npos)
			_loadLanguageXML(data, _user);
		else
			_loadLanguage(data, _user);

		delete data;
		return true;
	}

	void LanguageManager::_loadLanguageXML(IDataStream* _stream, bool _user)
	{
		xml::Document doc;
		// формат xml
		if (doc.open(_stream))
		{
			xml::ElementPtr root = doc.getRoot();
			if (root)
			{
				xml::ElementEnumerator tag = root->getElementEnumerator();
				while (tag.next("Tag"))
				{
					if (_user)
						mUserMapLanguage[tag->findAttribute("name")] = tag->getContent();
					else
						mMapLanguage[tag->findAttribute("name")] = tag->getContent();
				}
			}
		}
	}

	void LanguageManager::_loadLanguage(IDataStream* _stream, bool _user)
	{
		// формат txt
		std::string read;
		while (!_stream->eof())
		{
			_stream->readline(read, '\n');
			if (read.empty()) continue;

			// заголовок утф
			if ((uint8)read[0] == 0xEF && read.size() > 2)
			{
				read.erase(0, 3);
			}

			if (read[read.size()-1] == '\r') read.erase(read.size()-1, 1);
			if (read.empty()) continue;

			size_t pos = read.find_first_of(" \t");
			if (_user)
			{
				if (pos == std::string::npos) mUserMapLanguage[read] = "";
				else mUserMapLanguage[read.substr(0, pos)] = read.substr(pos+1, std::string::npos);
			}
			else
			{
				if (pos == std::string::npos) mMapLanguage[read] = "";
				else mMapLanguage[read.substr(0, pos)] = read.substr(pos+1, std::string::npos);
			}
		}
	}

	UString LanguageManager::replaceTags(const UString& _line)
	{
		//FIXME для совместимости
		/*if (mCurrentLanguageName.empty())
		{
			mDefaultName = "English";
			setCurrentLanguage(mDefaultName);
		}*/

		// вот хз, что быстрее, итераторы или математика указателей,
		// для непонятно какого размера одного символа UTF8
		UString line(_line);

		if (mMapLanguage.empty() && mUserMapLanguage.empty())
			return _line;

		UString::iterator end = line.end();
		for (UString::iterator iter=line.begin(); iter!=end; )
		{
			if (*iter == '#')
			{
				++iter;
				if (iter == end)
				{
					return line;
				}
				else
				{
					if (*iter != '{')
					{
						++iter;
						continue;
					}
					UString::iterator iter2 = iter;
					++iter2;
					while (true)
					{
						if (iter2 == end) return line;
						if (*iter2 == '}')
						{
							size_t start = iter - line.begin();
							size_t len = (iter2 - line.begin()) - start - 1;
							const UString& tag = line.substr(start + 1, len);

							bool find = true;
							MapLanguageString::iterator replace = mMapLanguage.find(tag);
							if (replace == mMapLanguage.end())
							{
								replace = mUserMapLanguage.find(tag);
								find = replace != mUserMapLanguage.end();
							}

							if (!find)
							{
								iter = line.insert(iter, '#') + size_t(len + 2);
								end = line.end();
								break;
							}

							iter = line.erase(iter - size_t(1), iter2 + size_t(1));
							size_t pos = iter - line.begin();
							line.insert(pos, replace->second);
							iter = line.begin() + pos + replace->second.length();
							end = line.end();
							if (iter == end) return line;
							break;

						}
						++iter2;
					};
				}
			}
			else
			{
				++iter;
			}
		}

		return line;
	}

	UString LanguageManager::getTag(const UString& _tag)
	{
		//FIXME
		//if (mCurrentLanguageName.empty()) setCurrentLanguage("English");

		MapLanguageString::iterator iter = mMapLanguage.find(_tag);
		if (iter == mMapLanguage.end())
		{
			iter = mUserMapLanguage.find(_tag);
			if (iter != mUserMapLanguage.end()) return iter->second;
			return _tag;
		}

		return iter->second;
	}

	const std::string& LanguageManager::getCurrentLanguage()
	{
		return mCurrentLanguageName;
	}

	void LanguageManager::addUserTag(const UString& _tag, const UString& _replace)
	{
		mUserMapLanguage[_tag] = _replace;
	}

	void LanguageManager::clearUserTags()
	{
		mUserMapLanguage.clear();
	}

	void LanguageManager::_loadSource(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next(XML_TYPE_SOURCE))
		{
			loadLanguage(node->getContent(), true);
		}
	}

	bool LanguageManager::loadUserTags(const std::string& _file)
	{
		return loadLanguage(_file, true);
	}

} // namespace MyGUI
