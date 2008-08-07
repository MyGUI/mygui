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

	Ogre::UTFString LanguageManager::replaceLanguage(const Ogre::UTFString & _line)
	{
		Ogre::UTFString line(_line);

		size_t pos = line.find("#{");

		return line;
	}

} // namespace MyGUI	
