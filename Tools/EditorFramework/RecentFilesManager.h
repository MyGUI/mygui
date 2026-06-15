/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _3302fbad_aab6_4b7b_b0a3_39a44fa633cd_
#define _3302fbad_aab6_4b7b_b0a3_39a44fa633cd_

#include <MyGUI.h>

namespace tools
{

	class MYGUI_EXPORT_DLL RecentFilesManager
	{
		MYGUI_SINGLETON_DECLARATION(RecentFilesManager);

	public:
		using VectorUString = std::vector<MyGUI::UString>;

		RecentFilesManager();

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
		size_t mMaxRecentFolders{0};
		VectorUString mRecentFiles;
		size_t mMaxRecentFiles{0};
	};

}

#endif
