/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	const std::string XML_TYPE("Language");

	INSTANCE_IMPLEMENT(LanguageManager);

	void LanguageManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LanguageManager::_load);

		mCurrentLanguage = mMapFile.end();


		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LanguageManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool LanguageManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LanguageManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		std::string def;

		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator pointer = _node->getNodeIterator();
		while (pointer.nextNode(XML_TYPE)) {

			// парсим атрибуты
			pointer->findAttribute("default", def);

			// берем детей и крутимся
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// парсим атрибуты
				std::string name(info->findAttribute("name"));
				std::string source(info->findAttribute("source"));

				// добавляем
				if (mMapFile.find(name) != mMapFile.end()) {
					MYGUI_LOG(Warning, "language '" << name << "' exist, erase old data");
				}
				mMapFile[name] = source;

			};
		};

		if ( ! def.empty()) setCurrentLanguage(def);
	}

	bool LanguageManager::setCurrentLanguage(const std::string & _name)
	{
		mCurrentLanguage = mMapFile.find(_name);
		if (mCurrentLanguage == mMapFile.end()) {
			MYGUI_LOG(Error, "Language '" << _name << "' is not found");
			return false;
		}

		return loadLanguage(mCurrentLanguage->second);
	}

	bool LanguageManager::loadLanguage(const std::string & _file)
	{
		mMapLanguage.clear();

		std::string group = Gui::getInstance().getResourceGroup();
		std::string file(group.empty() ? _file : helper::getResourcePath(_file, group));
		if (file.empty()) {
			MYGUI_LOG(Error, "file '" << _file << "' not found");
			return false;
		}

		if (!group.empty()) {
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(_file, group);
			_loadLanguage(stream);
			return true;
		}

		std::ifstream stream(file.c_str());
		if (false == stream.is_open()) {
			MYGUI_LOG(Error, "error open file '" << _file << "'");
			return false;
		}
		_loadLanguage(stream);
		stream.close();

		return true;
	}

	void LanguageManager::_loadLanguage(std::ifstream & _stream)
	{
		std::string read;
		while (false == _stream.eof()) {
			std::getline(_stream, read);
			if (read.empty()) continue;

			size_t pos = read.find_first_of(" \t");
			if (pos == std::string::npos) mMapLanguage[read] = "";
			else mMapLanguage[read.substr(0, pos)] = read.substr(pos+1, std::string::npos);
		};
	}

	void LanguageManager::_loadLanguage(const Ogre::DataStreamPtr& stream)
	{
		std::string read;
		while (false == stream->eof()) {
			read = stream->getLine (false);
			if (read.empty()) continue;

			size_t pos = read.find_first_of(" \t");
			if (pos == std::string::npos) mMapLanguage[read] = "";
			else mMapLanguage[read.substr(0, pos)] = read.substr(pos+1, std::string::npos);
		};
	}

	Ogre::UTFString LanguageManager::replaceTags(const Ogre::UTFString & _line)
	{
		// вот хз, что быстрее, итераторы или математика указателей,
		// для непонятно какого размера одного символа
		Ogre::UTFString line(_line);

		if (mMapLanguage.empty()) return _line;

		Ogre::UTFString::iterator end = line.end();
		for (Ogre::UTFString::iterator iter=line.begin(); iter!=end; ++iter) {
			if (*iter == '#') {
				++iter;
				if (iter == end) {
					return line;
				}
				else {
					if (*iter != '{') continue;
					Ogre::UTFString::iterator iter2 = iter;
					++iter2;
					while (true) {
						if (iter2 == end) return line;
						if (*iter2 == '}') {

							size_t start = iter - line.begin();
							size_t len = (iter2 - line.begin()) - start - 1;
							const Ogre::UTFString & tag = line.substr(start + 1, len);
							
							MapLanguageString::iterator replace = mMapLanguage.find(tag);
							if (replace == mMapLanguage.end()) {
								iter = line.insert(iter, '#') + len + 2;
								end = line.end();
								break;
							}
							else {
								iter = line.erase(iter-1, iter2+1);
								size_t pos = iter - line.begin();
								line.insert(pos, replace->second);
								iter = line.begin() + pos + replace->second.length();
								end = line.end();
								if (iter == end) return line;
								break;
							}

							iter = iter2;
							break;
						}
						++iter2;
					};
				}
			}
		}

		return line;
	}

	Ogre::UTFString LanguageManager::getTag(const Ogre::UTFString & _tag)
	{
		MapLanguageString::iterator iter = mMapLanguage.find(_tag);
		if (iter == mMapLanguage.end()) {
			return _tag;
		}
		return iter->second;
	}

} // namespace MyGUI	
