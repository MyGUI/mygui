#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "BaseManager.h"
#include "StateController.h"
#include "EditorState.h"

namespace tools
{
	class Application :
		public base::BaseManager,
		public MyGUI::Singleton<Application>,
		public StateController
	{
	public:
		Application();
		virtual ~Application();

		virtual void createScene();
		virtual void destroyScene();
		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

		virtual void prepare();
		virtual void onFileDrop(const std::wstring& _filename);
		virtual bool onWinodwClose(size_t _handle);

		typedef std::vector<std::wstring> VectorWString;
		const VectorWString& getParams() { return mParams; }

		void setCaption(const MyGUI::UString& _value);

		virtual void resumeState();

	private:
		void commandQuitApp(const MyGUI::UString& _commandName);
		void commandStatisticInfo(const MyGUI::UString& _commandName);
		void commandFocusVisible(const MyGUI::UString& _commandName);

		int toGrid(int _x);

		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);
		void notifyChangeSelectedWidget(MyGUI::Widget* _widget);

	private:
		// last click for depth selecting
		int mLastClickX;
		int mLastClickY;
		int mSelectDepth;

		VectorWString mParams;
		std::string mLocale;

		int mGridStep;

		EditorState* mEditorState;
	};

} // namespace tools

#endif // __APPLICATION_H__
