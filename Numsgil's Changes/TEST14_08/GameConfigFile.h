#pragma once

#include <Ogre.h>

class GameConfigFile : public Ogre::ConfigFile
{
public:
	// установка значения
	void SetSetting(const Ogre::String & key, const Ogre::String & value, const Ogre::String & section = "");
	// сохранение в файл
	void SaveToFile(const Ogre::String & FileName);
	// удаление элемента
	void clearSetting(const Ogre::String & key, const Ogre::String & section = "");
	// очистка секции
	void clearSection(const Ogre::String & section);

	GameConfigFile() {};
	virtual ~GameConfigFile() {};
};