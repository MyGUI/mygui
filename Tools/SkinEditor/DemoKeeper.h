/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "MainPane.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager,
		public MyGUI::Singleton<DemoKeeper>
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);

		void setChanges(bool _value);
		void setFileName(const MyGUI::UString& _value);

	private:
		virtual void setupResources();
		void updateCaption();

	private:
		tools::MainPane* mMainPane;
		bool mChanges;
		MyGUI::UString mFileName;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
