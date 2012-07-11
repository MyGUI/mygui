/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SETTINGS_MANAGER_H__
#define __SETTINGS_MANAGER_H__

#include <MyGUI.h>
#include "SettingsSector.h"

namespace tools
{
	typedef MyGUI::delegates::CMultiDelegate2<const MyGUI::UString&, const MyGUI::UString&> EventSectorSettingsChanged;

	class SettingsManager :
		public MyGUI::Singleton<SettingsManager>
	{
	public:
		SettingsManager();
		virtual ~SettingsManager();

		void initialise(const MyGUI::UString& _userFileName);
		void shutdown();

		SettingsSector* getSector(const MyGUI::UString& _sectorName);

		EventSectorSettingsChanged eventSettingsChanged;

	private:
		void loadSettings(const MyGUI::UString& _fileName, bool _internal);
		void saveSettings(const MyGUI::UString& _fileName);

		void destroyAllSectors();
		void saveSectors(MyGUI::xml::ElementPtr _rootNode);
		void loadSector(MyGUI::xml::ElementPtr _sectorNode);

		void notifySettingsChanged(SettingsSector* _sector, const MyGUI::UString& _propertyName);

	private:
		VectorSettingsSector mSettings;
		MyGUI::UString mFileName;
		MyGUI::UString mUserFileName;
	};

} // namespace tools

#endif // __SETTINGS_MANAGER_H__
