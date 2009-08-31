/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "MyGUI_TextView.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool keyPressed( const OIS::KeyEvent &arg );

		void setCursorPosition(size_t _index);

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
		MyGUI::IFont* mFont;
		MyGUI::Align mTextAlign;
		MyGUI::IntPoint mOffsetView;
		
		TextView mTextView;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
