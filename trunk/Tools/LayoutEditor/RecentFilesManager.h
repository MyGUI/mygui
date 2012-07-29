/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _c8874940_90f6_4b60_868d_8afe9e6d2d47_
#define _c8874940_90f6_4b60_868d_8afe9e6d2d47_

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

		void addRecentProject(const MyGUI::UString& _fileName);
		const VectorUString& getRecentProjects() const;

	private:
		void checkArray(VectorUString& _array, size_t _maxElements);

	private:
		MyGUI::UString mRecentFolder;
		VectorUString mRecentFolders;
		size_t mMaxRecentFolders;
		VectorUString mRecentFiles;
		size_t mMaxRecentFiles;
		VectorUString mRecentProjects;
		size_t mMaxRecentProjects;
	};

}

#endif
