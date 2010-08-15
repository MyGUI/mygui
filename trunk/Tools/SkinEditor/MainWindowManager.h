/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MAIN_WINDOW_MANAGER_H__
#define __MAIN_WINDOW_MANAGER_H__

#include <MyGUI.h>

namespace tools
{

	class MainWindowManager :
		public MyGUI::Singleton<MainWindowManager>
	{
	public:
		MainWindowManager();
		virtual ~MainWindowManager();

		void initialise();
		void shutdown();

		void setChanges(bool _value);
		void setFileName(const MyGUI::UString& _value);

	private:
		void updateCaption();

	private:
		bool mChanges;
		MyGUI::UString mFileName;
	};

} // namespace tools

#endif // __MAIN_WINDOW_MANAGER_H__
