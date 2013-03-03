/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_DataStreamHolder.h"

namespace MyGUI
{

	template <> LanguageManager* Singleton<LanguageManager>::msInstance = nullptr;
	template <> const char* Singleton<LanguageManager>::mClassTypeName = "LanguageManager";

	LanguageManager::LanguageManager() :
		mIsInitialise(false),
		mXmlLanguageTagName("Language")
	{
	}

	void LanguageManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		ResourceManager::getInstance().registerLoadXmlDelegate(mXmlLanguageTagName) = newDelegate(this, &LanguageManager::_load);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void LanguageManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		ResourceManager::getInstance().unregisterLoadXmlDelegate(mXmlLanguageTagName);

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void LanguageManager::_load(xml::ElementPtr _node, const std::string& _file, Version _version)
	{
		std::string default_lang;
		bool event_change = false;

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator root = _node->getElementEnumerator();
		while (root.next(mXmlLanguageTagName))
		{
			// парсим атрибуты
			root->findAttribute("default", default_lang);

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
						loadLanguage(source_info->getContent(), true);
					}
				}
				// добавляем в карту языков
				else
				{
					xml::ElementEnumerator source_info = info->getElementEnumerator();
					while (source_info.next("Source"))
					{
						std::string file_source = source_info->getContent();
						// добавляем в карту
						mMapFile[name].push_back(file_source);

						// если добавляемый файл для текущего языка, то подгружаем и оповещаем
						if (name == mCurrentLanguageName)
						{
							loadLanguage(file_source, false);
							event_change = true;
						}
					}
				}

			}
		}

		if (!default_lang.empty())
			setCurrentLanguage(default_lang);
		else if (event_change)
			eventChangeLanguage(mCurrentLanguageName);
	}

	void LanguageManager::setCurrentLanguage(const std::string& _name)
	{
		MapListString::iterator item = mMapFile.find(_name);
		if (item == mMapFile.end())
		{
			MYGUI_LOG(Error, "Language '" << _name << "' is not found");
			return;
		}

		mMapLanguage.clear();
		mCurrentLanguageName = _name;

		for (VectorString::const_iterator iter = item->second.begin(); iter != item->second.end(); ++iter)
		{
			loadLanguage(*iter, false);
		}

		eventChangeLanguage(mCurrentLanguageName);
	}

	bool LanguageManager::loadLanguage(const std::string& _file, bool _user)
	{
		DataStreamHolder data = DataManager::getInstance().getData(_file);
		if (data.getData() == nullptr)
		{
			MYGUI_LOG(Error, "file '" << _file << "' not found");
			return false;
		}

		if (_file.find(".xml") != std::string::npos)
			_loadLanguageXML(data.getData(), _user);
		else
			_loadLanguage(data.getData(), _user);

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

			if (read[read.size() - 1] == '\r') read.erase(read.size() - 1, 1);
			if (read.empty()) continue;

			size_t pos = read.find_first_of(" \t");
			if (_user)
			{
				if (pos == std::string::npos) mUserMapLanguage[read] = "";
				else mUserMapLanguage[read.substr(0, pos)] = read.substr(pos + 1, std::string::npos);
			}
			else
			{
				if (pos == std::string::npos) mMapLanguage[read] = "";
				else mMapLanguage[read.substr(0, pos)] = read.substr(pos + 1, std::string::npos);
			}
		}
	}

	UString LanguageManager::replaceTags(const UString& _line)
	{
		UString result(_line);

		bool replace = false;
		do
		{
			result = replaceTagsPass(result, replace);
		}
		while (replace);

		return result;
	}

	UString LanguageManager::getTag(const UString& _tag)
	{
		MapLanguageString::iterator iter = mMapLanguage.find(_tag);
		if (iter != mMapLanguage.end())
		{
			return iter->second;
		}

		MapLanguageString::iterator iterUser = mUserMapLanguage.find(_tag);
		if (iterUser != mUserMapLanguage.end())
		{
			return iterUser->second;
		}

		return _tag;
	}

	const std::string& LanguageManager::getCurrentLanguage() const
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

	bool LanguageManager::loadUserTags(const std::string& _file)
	{
		return loadLanguage(_file, true);
	}

	UString LanguageManager::replaceTagsPass(const UString& _line, bool& _replaceResult)
	{
		_replaceResult = false;

		// вот хз, что быстрее, итераторы или математика указателей,
		// для непонятно какого размера одного символа UTF8
		UString line(_line);

		UString::iterator end = line.end();
		for (UString::iterator iter = line.begin(); iter != end; )
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
						if (iter2 == end)
							return line;

						if (*iter2 == '}')
						{
							size_t start = iter - line.begin();
							size_t len = (iter2 - line.begin()) - start - 1;
							const UString& tag = line.substr(start + 1, len);
							UString replacement;

							bool find = true;
							// try to find in loaded from resources language strings
							MapLanguageString::iterator replace = mMapLanguage.find(tag);
							if (replace != mMapLanguage.end())
							{
								replacement = replace->second;
							}
							else
							{
								// try to find in user language strings
								replace = mUserMapLanguage.find(tag);
								if (replace != mUserMapLanguage.end())
								{
									replacement = replace->second;
								}
								else
								{
									find = false;
								}
							}

							// try to ask user if event assigned or use #{_tag} instead
							if (!find)
							{
								if (!eventRequestTag.empty())
								{
									eventRequestTag(tag, replacement);
								}
								else
								{
									iter = line.insert(iter, '#') + size_t(len + 2);
									end = line.end();
									break;
								}
							}

							_replaceResult = true;

							iter = line.erase(iter - size_t(1), iter2 + size_t(1));
							size_t pos = iter - line.begin();
							line.insert(pos, replacement);
							iter = line.begin() + pos + replacement.length();
							end = line.end();
							if (iter == end)
								return line;
							break;
						}
						++iter2;
					}
				}
			}
			else
			{
				++iter;
			}
		}

		return line;
	}

} // namespace MyGUI
