/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __RECENT_FILES_MANAGER_H__
#define __RECENT_FILES_MANAGER_H__

#include <MyGUI.h>

namespace tools
{
	class RecentFilesManager :
		public MyGUI::Singleton<RecentFilesManager>
	{
	public:
		typedef std::vector<MyGUI::UString> VectorUString;

		RecentFilesManager();
		virtual ~RecentFilesManager();

		void initialise();
		void shutdown();

		void addRecentFolder(const MyGUI::UString& _folder);
		const VectorUString& getRecentFolders() const;

		void setRecentFolder(const MyGUI::UString& _value);
		const MyGUI::UString& getRecentFolder() const;

		void addRecentFile(const MyGUI::UString& _fileName);
		const VectorUString& getRecentFiles() const;

	private:
		void checkArray(VectorUString& _array, size_t _maxElements);

	private:
		MyGUI::UString mRecentFolder;
		VectorUString mRecentFolders;
		size_t mMaxRecentFolders;
		VectorUString mRecentFiles;
		size_t mMaxRecentFiles;
	};

} // namespace tools

#endif // __RECENT_FILES_MANAGER_H__
