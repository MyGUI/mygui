/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
//#include "PanelView/PanelCell.h"
//#include "PanelView/PanelViewWindow.h"

#include "PanelViewWindow.h"
#include "PanelDirector.h"
#include "PanelDynamic.h"
#include "PanelStatic.h"

namespace demo
{

	struct LineInfo
	{
		LineInfo() : newline(false), width(0), count(0) { }
		void clear() { width = 0; count = 0; simbols.clear(); }
		int width;
		size_t count;
		std::vector<int> simbols;
		bool newline;
	};

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyChangePanels(int _key, size_t _value);
		virtual bool mouseMoved( const OIS::MouseEvent &arg );

		size_t getCursorPosition(int _left, int _top);
		void setCursorPosition(size_t _position);

		MyGUI::IntSize getSizeTextNativeHeight(const MyGUI::UString& _text, MyGUI::FontPtr _font);
		MyGUI::IntSize getSizeTextScaleHeight(const MyGUI::UString& _text, MyGUI::FontPtr font, int _height);
		MyGUI::IntSize getSizeText(const MyGUI::UString& _text, const std::string& _fontname, int _height);

	private:
		typedef std::vector<LineInfo> VectorLineInfo;
		VectorLineInfo mLineInfo;
		size_t mLength;
		MyGUI::StaticTextPtr mText;
		MyGUI::WidgetPtr mCursor;
		size_t mCursorPosition;
		
		PanelViewWindow* mView;
		PanelDirector* mPanelDirector;
		PanelDynamic* mPanelDynamic;
		PanelStatic* mPanelStatic;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
