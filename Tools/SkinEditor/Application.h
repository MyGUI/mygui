/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "EditorState.h"

namespace tools
{

	class Application :
		public base::BaseManager,
		public MyGUI::Singleton<Application>
	{
	public:
		Application();
		virtual ~Application();

		virtual void createScene();
		virtual void destroyScene();

		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);
		virtual void prepare();

		void setCaption(const MyGUI::UString& _value);

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams() { return mParams; }

	protected:
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);

	private:
		virtual void setupResources();

	private:
		EditorState* mEditorState;

		std::string mLocale;
		VectorWString mParams;
	};

} // namespace tools

#endif // __DEMO_KEEPER_H__
