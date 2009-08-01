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

namespace MyGUI
{

	const std::string XML_TYPE("Language");

	MYGUI_INSTANCE_IMPLEMENT(LanguageManager);

	void LanguageManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LanguageManager::_load);

		mCurrentLanguage = mMapFile.end();


		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LanguageManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		ResourceManager::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool LanguageManager::load(const std::string& _file/*, const std::string& _group*/)
	{
		return ResourceManager::getInstance()._loadImplement(_file, /*_group, */true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LanguageManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		std::string def;

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next(XML_TYPE))
		{
			// парсим атрибуты
			root->findAttribute("default", def);

			// берем детей и крутимся
			xml::ElementEnumerator info = root->getElementEnumerator();
			while (info.next("Info"))
			{
				// парсим атрибуты
				std::string name(info->findAttribute("name"));

				// доюавляем в карту пользователя
				if (name.empty())
				{
					xml::ElementEnumerator source_info = info->getElementEnumerator();
					while (source_info.next("Source"))
					{
						loadLanguage(source_info->getContent(), /*ResourceManager::getInstance().getResourceGroup(), */true);
					};
				}
				// добавляем в карту языков
				else
				{
					MapListString::iterator lang = mMapFile.find(name);
					if (lang == mMapFile.end())
					{
						lang = mMapFile.insert(std::make_pair(name, VectorString())).first;
					}

					xml::ElementEnumerator source_info = info->getElementEnumerator();
					while (source_info.next("Source"))
					{
						lang->second.push_back(source_info->getContent());
					};
				}

			};
		};

		if ( ! def.empty()) setCurrentLanguage(def);
	}

	bool LanguageManager::setCurrentLanguage(const std::string& _name)
	{
		mCurrentLanguage = mMapFile.find(_name);
		if (mCurrentLanguage == mMapFile.end())
		{
			MYGUI_LOG(Error, "Language '" << _name << "' is not found");
			return false;
		}

		loadLanguage(mCurrentLanguage->second/*, ResourceManager::getInstance().getResourceGroup()*/);
		eventChangeLanguage(mCurrentLanguage->first);
		return true;
	}

	void LanguageManager::loadLanguage(const VectorString& _list/*, const std::string& _group*/)
	{
		mMapLanguage.clear();

		for (VectorString::const_iterator iter=_list.begin(); iter!=_list.end(); ++iter)
		{
			loadLanguage(*iter/*, _group*/);
		}
	}

	bool LanguageManager::loadLanguage(const std::string& _file, bool _user)
	{
		//if (true)
		//{
			IDataStream* data = DataManager::getInstance().getData(_file);
			if (data == nullptr)
			{
				MYGUI_LOG(Error, "file '" << _file << "' not found");
				return false;
			}

			// проверяем на сигнатуру utf8
			/*uint8* buff = data->getData();
			if (data->getSize() < 3 || buff[0] != 0xEF || buff[1] != 0xBB || buff[2] != 0xBF)
			{
				MYGUI_LOG(Error, "file '" << _file << "' is not UTF8 format");
				delete data;
				return false;
			}

			std::string tmp((const char*)buff+3, data->getSize()-3);
			std::istringstream stream(tmp);*/

			_loadLanguage(data, _user);

			delete data;
			return true;
		//}

		/*std::ifstream stream(_file.c_str());
		if (false == stream.is_open())
		{
			MYGUI_LOG(Error, "error open file '" << _file << "'");
			return false;
		}

		// проверяем на сигнатуру utf8
		uint32 sign = 0;
		stream.read((char*)&sign, 3);
		if (sign != 0x00BFBBEF)
		{
			MYGUI_LOG(Error, "file '" << _file << "' is not UTF8 format");
			stream.close();
			return false;
		}

		_loadLanguage(stream, _user);
		stream.close();*/

		return true;
	}

	void LanguageManager::_loadLanguage(IDataStream* _stream, bool _user)
	{
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
		};
	}

	UString LanguageManager::replaceTags(const UString& _line)
	{
		// вот хз, что быстрее, итераторы или математика указателей,
		// для непонятно какого размера одного символа UTF8
		UString line(_line);

		if (mMapLanguage.empty() && mUserMapLanguage.empty()) return _line;

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
		MapLanguageString::iterator iter = mMapLanguage.find(_tag);
		if (iter == mMapLanguage.end())
		{
			iter = mUserMapLanguage.find(_tag);
			if (iter != mUserMapLanguage.end()) return iter->second;
			return _tag;
		}

		return iter->second;
	}

	bool LanguageManager::isLanguageExist(const std::string& _name)
	{
		return mMapFile.find(_name) != mMapFile.end();
	}

	std::string LanguageManager::getCurrentLanguage()
	{
		return mCurrentLanguage != mMapFile.end() ? mCurrentLanguage->first : "";
	}

	void LanguageManager::addUserTag(const UString& _tag, const UString& _replace)
	{
		mUserMapLanguage[_tag] = _replace;
	}

	void LanguageManager::clearUserTags()
	{
		mUserMapLanguage.clear();
	}

} // namespace MyGUI
