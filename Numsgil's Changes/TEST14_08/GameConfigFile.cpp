#include "GameConfigFile.h"

using namespace Ogre;
using namespace std;

void GameConfigFile::SetSetting(const String & key, const String & value, const String & section)
{
	SettingsBySection::iterator seci = mSettings.find(section);
	if (seci == mSettings.end()) {
		SettingsMultiMap *currentSettings = new SettingsMultiMap();
		mSettings[section] = currentSettings;
		seci = mSettings.find(section);
	}

	SettingsMultiMap &m = *seci->second;
	bool flag = 0;
	for (SettingsMultiMap::iterator i = m.begin(); i != m.end(); ++i) {
		if ((!flag)&&((*i).first == key)) {
			(*i).second = value;
			flag = 1;
		}
	}
	if (!flag) m.insert(std::multimap<String, String>::value_type(key, value));
};
// сохранение в файл
void GameConfigFile::SaveToFile(const String & FileName)
{
	std::ofstream fp;
	fp.open(FileName.c_str(), ios_base::out | ios_base::trunc);
	if (!fp) OGRE_EXCEPT(Exception:: ERR_FILE_NOT_FOUND, "'" + FileName + "' file not found!", "GameConfigFile::SaveToFile");

	SettingsBySection::const_iterator seci =  mSettings.begin();
	for (; seci != mSettings.end(); seci++) {
		SettingsMultiMap &m = *seci->second;
		if (seci->first != StringUtil::BLANK) fp << "\n[" << seci->first <<  "]" << endl;
		SettingsMultiMap::iterator it =  m.begin();
		for (; it != m.end(); it++) fp << it->first << "=" << it->second << endl;
	}
	fp.close();
};
// удаление элемента
void GameConfigFile::clearSetting(const String & key, const String & section)
{
	SettingsBySection::iterator seci = mSettings.find(section);
	if (seci == mSettings.end()) {
		//_OUT("No setting called %s", key.c_str());
	} else (*seci).second->erase(key);
};
// очистка секции
void GameConfigFile::clearSection(const String & section)
{
	SettingsBySection::iterator seci = mSettings.find(section);
	if (seci == mSettings.end()) {
		//_OUT("No section called %s", section.c_str());
	} else {
		(*seci).second->clear();
		delete (*seci).second;
		mSettings.erase(section);
	}
};