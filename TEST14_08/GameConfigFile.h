#pragma once

#include <OgreString.h>
#include <OgreConfigFile.h>

class GameConfigFile : public Ogre::ConfigFile
{
public:
	// ��������� ��������
	void SetSetting(const Ogre::String & key, const Ogre::String & value, const Ogre::String & section = "");
	// ���������� � ����
	void SaveToFile(const Ogre::String & FileName);
	// �������� ��������
	void clearSetting(const Ogre::String & key, const Ogre::String & section = "");
	// ������� ������
	void clearSection(const Ogre::String & section);

	GameConfigFile();
	virtual ~GameConfigFile();
};