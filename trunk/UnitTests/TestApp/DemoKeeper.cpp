/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "MyGUI_EditText2.h"

namespace demo
{

	MyGUI::EditPtr edit = 0;
	void DemoKeeper::createScene()
	{
		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::EditText2>("BasisSkin");

		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(100, 100, 850, 280), MyGUI::Align::Default, "Overlapped");
		edit = window->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(0, 0, 700, 200), MyGUI::Align::Stretch);
		edit->setEditWordWrap(true);
		edit->setTextAlign(MyGUI::Align::Default);
		edit->setEditMultiLine(true);
		//edit->setEditMultiLine(true);
		edit->setTextColour(MyGUI::Colour::Black);
		edit->setFontName("font_Micro.11");
		//edit->setInvertSelected(false);
		edit->setTabPrinting(true);
		//edit->setFontHeight(20);
		edit->setCaption(L"!\"##$%&'()*+,-./0123456789\n#0000FFABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n[\\]^_`{|}~\n#00FF00ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß¨\n#FF0000àáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ¸");
		//edit->setCaption("testtest\n#00FF00testtes\nt#0000FFtesttest\ntesttesttesttest\n#00FF00testtes\nt#0000FFtesttest\ntesttesttesttest\n#00FF00testtes\nt#0000FFtesttest\ntesttesttesttest\n#00FF00testtes\nt#0000FFtesttest\ntesttest");

		mFont = MyGUI::FontManager::getInstance().getByName("Default");
		mFontHeight = 50;
		mCaption = "test\nte#FF00FFst\ntest";
		mOffset.set(580, 580);

		mWidget = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(), MyGUI::Align::Default, "Overlapped");

		mCursor = mGUI->createWidget<MyGUI::StaticText>("MarkerGreen", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default, "Statistic");

		//MyGUI::IFont* font = MyGUI::FontManager::getInstance().getByName("Default");
		//font->getTextureFont()->saveToFile("Default.png");

		mTextAlign = MyGUI::Align::Center;
		mCursorPosition = 0;
		setCursorPosition(mCursorPosition);

		update();
	}

	void DemoKeeper::destroyScene()
	{
		mFont = nullptr;
	}

	void DemoKeeper::update()
	{
		mTextView.update(mCaption, mFont, mFontHeight, mTextAlign, MyGUI::VertexColourType::ColourABGR, -1);
		const MyGUI::IntSize& size = mTextView.getViewSize();
		mWidget->setCoord(mOffset.left, mOffset.top, size.width + 2, size.height);

		while (mChars.size() < mTextView.getTextLength())
		{
			MyGUI::StaticImagePtr image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(), MyGUI::Align::Default, "Popup");
			image->setImageTexture(mFont->getTextureFont()->getName());
			mChars.push_back(image);
		}

		int top = mOffsetView.top;
		int left = mOffsetView.left;
		size_t index = 0;
		for (VectorLineInfo::const_iterator line=mTextView.getData().begin(); line!=mTextView.getData().end(); ++line)
		{
			left = line->offset + mOffsetView.left;
			for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
			{
				if (sim->isColour())
					continue;

				if (left + sim->getWidth() <= 0
					|| left >= mTextView.getViewSize().width
					|| top + mFontHeight <= 0
					|| top >= mTextView.getViewSize().height)
				{
					left += sim->getWidth();
					continue;
				}

				mChars[index]->setCoord(left + mOffset.left, top + mOffset.top, sim->getWidth(), mFontHeight);
				mChars[index]->_setUVSet(sim->getUVRect());
				left += sim->getWidth();
				index++;
			}
			top += mFontHeight;
		}

		while (index < mChars.size())
		{
			mChars[index]->setCoord(0, 0, 0, 0);
			index++;
		}
	}

	bool DemoKeeper::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (mFont == nullptr) return false;

		mOffsetView = MyGUI::IntPoint(arg.state.X.abs - mOffset.left, arg.state.Y.abs - mOffset.top);
		update();

		//int pos = arg.state.X.abs - mOffset.left;
		//if (pos < 0) pos = 0;
		//if (pos >= edit->getVScrollRange()) pos = edit->getVScrollRange() - 1;
		//edit->setVScrollPosition(pos);
		//mCursorPosition = mTextView.getCursorPosition(arg.state.X.abs - mOffset.left, arg.state.Y.abs - mOffset.top);
		//setCursorPosition(mCursorPosition);
		return base::BaseManager::mouseMoved(arg);
	}

	void DemoKeeper::setCursorPosition(size_t _position)
	{
		const MyGUI::IntPoint& point = mTextView.getCursorPoint(_position);
		mCursor->setCoord(point.left + mOffset.left, point.top + mOffset.top, 2, mFontHeight);
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		if (mFont == nullptr) return false;
		if (MyGUI::InputManager::getInstance().getKeyFocusWidget() != nullptr)
			return base::BaseManager::keyPressed(arg);

		if (arg.key == OIS::KC_ESCAPE)
		{
		}
		else if (arg.key == OIS::KC_SPACE)
		{
			if (edit)
			{
				MyGUI::MYGUI_OUT(edit->getHScrollRange());
				MyGUI::MYGUI_OUT(edit->getHScrollPosition());
				edit->setTextColour(MyGUI::Colour::Red);
			}
		}
		else if (arg.key == OIS::KC_LEFT)
		{
			if (mCursorPosition != 0)
			{
				mCursorPosition--;
				setCursorPosition(mCursorPosition);
			}
			else
			{
				MyGUI::MYGUI_OUT("min");
			}
		}
		else if (arg.key == OIS::KC_RIGHT)
		{
			if (mCursorPosition < mTextView.getTextLength())
			{
				mCursorPosition++;
				setCursorPosition(mCursorPosition);
			}
			else
			{
				MyGUI::MYGUI_OUT("max");
			}
		}
		else if (arg.key == OIS::KC_UP)
		{
			if (mCursorPosition > 0)
			{
				MyGUI::IntPoint point = mTextView.getCursorPoint(mCursorPosition);
				point.top -= mFontHeight;
				size_t position = mTextView.getCursorPosition(point);
				if (position == mCursorPosition && mCursorPosition != 0)
					mCursorPosition = 0;
				else if (position != mCursorPosition)
					mCursorPosition  = position;
				setCursorPosition(mCursorPosition);
			}
			else
			{
				MyGUI::MYGUI_OUT("min");
			}
		}
		else if (arg.key == OIS::KC_DOWN)
		{
			if (mCursorPosition < mTextView.getTextLength())
			{
				MyGUI::IntPoint point = mTextView.getCursorPoint(mCursorPosition);
				point.top += mFontHeight;
				size_t position = mTextView.getCursorPosition(point);
				if (position == mCursorPosition && mCursorPosition != mTextView.getTextLength())
					mCursorPosition = mTextView.getTextLength();
				else if (position != mCursorPosition)
					mCursorPosition  = position;
				setCursorPosition(mCursorPosition);
			}
			else
			{
				MyGUI::MYGUI_OUT("max");
			}
		}
		else if (arg.key == OIS::KC_DELETE)
		{
			if (mTextView.getTextLength() != 0 && mCursorPosition < mTextView.getTextLength())
			{
				mCaption.erase(mCaption.begin() + mCursorPosition);
				update();
				setCursorPosition(mCursorPosition);
			}
			else
				MyGUI::MYGUI_OUT("error");
		}
		else if (arg.key == OIS::KC_BACK)
		{
			if (mTextView.getTextLength() != 0 && mCursorPosition != 0)
			{
				mCursorPosition --;
				mCaption.erase(mCaption.begin() + mCursorPosition);
				update();
				setCursorPosition(mCursorPosition);
			}
			else
				MyGUI::MYGUI_OUT("error");
		}
		else if (arg.text != 0)
		{
			mCaption.insert(mCaption.begin() + mCursorPosition, arg.text);
			update();
			mCursorPosition++;
			setCursorPosition(mCursorPosition);
		}

		return base::BaseManager::keyPressed(arg);
	}

} // namespace demo
