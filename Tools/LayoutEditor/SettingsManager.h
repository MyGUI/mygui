/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SETTINGS_MANAGER_H__
#define __SETTINGS_MANAGER_H__

#include <MyGUI.h>

namespace tools
{

	typedef std::vector<MyGUI::UString> VectorUString;

	class SettingsManager :
		public MyGUI::Singleton<SettingsManager>
	{
	public:
		SettingsManager();
		virtual ~SettingsManager();

		void initialise();
		void shutdown();

		const VectorUString& getRecentFiles() { return mRecentFiles; }
		const VectorUString& getAdditionalPaths() { return mAdditionalPaths; }

		void addRecentFile(const MyGUI::UString& _fileName);

	private:
		void loadSettings(const MyGUI::UString& _fileName, bool _internal);
		void saveSettings(const MyGUI::UString& _fileName);

	private:
		std::vector<MyGUI::UString> mRecentFiles;
		std::vector<MyGUI::UString> mAdditionalPaths;
	};

} // namespace tools

#endif // __SETTINGS_MANAGER_H__
