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

#include "MyGUI_TextView.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyChangePanels(int _key, size_t _value);
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool keyPressed( const OIS::KeyEvent &arg );

		//size_t getCursorPosition(int _left, int _top);
		void setCursorPosition(size_t _position);
		//MyGUI::IntPoint getCursorPoint(size_t _position);

		void update();

	private:
		int mFontHeight;
		MyGUI::WidgetPtr mCursor;
		MyGUI::WidgetPtr mWidget;
		size_t mCursorPosition;
		std::string mCaption;
		MyGUI::IntPoint mOffset;
		typedef std::vector<MyGUI::StaticImagePtr> VectorWidget;
		VectorWidget mChars;
		MyGUI::FontPtr mFont;
		MyGUI::Align mTextAlign;
		
		PanelViewWindow* mView;
		PanelDirector* mPanelDirector;
		PanelDynamic* mPanelDynamic;
		PanelStatic* mPanelStatic;

		TextView mTextView;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
