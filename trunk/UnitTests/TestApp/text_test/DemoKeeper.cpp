/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_PanelView");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Panel View control implementation.");

		mView = new PanelViewWindow();
		mPanelDirector = new PanelDirector();
		mPanelDynamic = new PanelDynamic();
		mPanelStatic = new PanelStatic();

		mPanelDirector->eventChangePanels = MyGUI::newDelegate(this, &DemoKeeper::notifyChangePanels);
		mView->getPanelView()->addItem(mPanelDirector);
		mView->getPanelView()->addItem(mPanelDynamic);
		mView->getPanelView()->addItem(mPanelStatic);

		mFont = MyGUI::FontManager::getInstance().getByName("Default");
		mFontHeight = 50;
		mCaption = "test\ntest\ntest";
		mOffset.set(80, 80);

		mWidget = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(), MyGUI::Align::Default, "Overlapped");

		mCursor = mGUI->createWidget<MyGUI::StaticText>("MarkerGreen", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default, "Statistic");

		mTextAlign = MyGUI::Align::Center;
		mCursorPosition = 0;
		setCursorPosition(mCursorPosition);

		update();
	}

	void DemoKeeper::destroyScene()
	{
		mView->getPanelView()->removeAllItems();
		delete mView;

		delete mPanelDirector;
		delete mPanelDynamic;
		delete mPanelStatic;

		mFont.setNull();
	}

	void DemoKeeper::update()
	{
		mTextView.update(mCaption, mFont, mFontHeight, mTextAlign, -1);
		const MyGUI::IntSize& size = mTextView.getViewSize();
		mWidget->setCoord(mOffset.left, mOffset.top, size.width + 2, size.height);

		while (mChars.size() < mTextView.getTextLength())
		{
			MyGUI::StaticImagePtr image = mWidget->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(), MyGUI::Align::Default);
			image->setImageTexture(mFont->getTextureFont()->getName());
			mChars.push_back(image);
		}

		int top = 0;
		int left = 0;
		size_t index = 0;
		for (VectorLineInfo::const_iterator line=mTextView.getData().begin(); line!=mTextView.getData().end(); ++line)
		{
			left = line->offset;
			for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
			{
				mChars[index]->setCoord(left, top, sim->width, mFontHeight);
				mChars[index]->_setUVSet(sim->rect);
				left += sim->width;
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

	void DemoKeeper::notifyChangePanels(int _key, size_t _value)
	{
		if (_key == EVENT_SHOW_STATIC)
		{
			mView->getPanelView()->setItemShow(mPanelStatic, _value != 0);
		}
		else if (_key == EVENT_SHOW_DYNAMIC)
		{
			mView->getPanelView()->setItemShow(mPanelDynamic, _value != 0);
		}
		else if (_key == EVENT_COUNT_DYNAMIC)
		{
			mPanelDynamic->setVisibleCount(_value);
		}
	}

	bool DemoKeeper::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (mFont.isNull()) return false;

		mCursorPosition = mTextView.getCursorPosition(arg.state.X.abs - mOffset.left, arg.state.Y.abs - mOffset.top);
		setCursorPosition(mCursorPosition);
		return base::BaseManager::mouseMoved(arg);
	}

	/*size_t DemoKeeper::getCursorPosition(int _left, int _top)
	{
		const int height = mFontHeight;
		size_t result = 0;
		int top = 0;

		for (VectorLineInfo::const_iterator line=mTextView.getData().begin(); line!=mTextView.getData().end(); ++line)
		{
			// это последн€€ строка
			bool lastline = !(line + 1 != mTextView.getData().end());

			// наша строчка
			if (top + height > _top || lastline)
			{
				top += height;
				int left = line->offset;
				int count = 0;

				// ищем символ
				for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
				{
					if ((left + (sim->width / 2)) > _left)
					{
						break;
					}
					left += sim->width;
					count ++;
				}

				result += count;
				break;
			}

			if (!lastline)
			{
				top += height;
				result += line->count + 1;
			}
		}

		return result;
	}*/

	void DemoKeeper::setCursorPosition(size_t _position)
	{
		const MyGUI::IntPoint& point = mTextView.getCursorPoint(_position);
		mCursor->setCoord(point.left + mOffset.left, point.top + mOffset.top, 2, mFontHeight);
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		if (mFont.isNull()) return false;

		if (arg.key == OIS::KC_LEFT)
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
				size_t position = mTextView.getCursorPosition(point.left, point.top);
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
				size_t position = mTextView.getCursorPosition(point.left, point.top);
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

	/*MyGUI::IntPoint DemoKeeper::getCursorPoint(size_t _position)
	{
		if (_position >= mTextView.getTextLength() + 1) _position = mTextView.getTextLength();

		size_t position = 0;
		int top = 0;
		int left = 0;
		for (VectorLineInfo::const_iterator line=mTextView.getData().begin(); line!=mTextView.getData().end(); ++line)
		{
			left = line->offset;
			if (position + line->count >= _position)
			{
				for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
				{
					if (position == _position)
					{
						break;
					}

					position ++;
					left += sim->width;
				}
				break;
			}
			position += line->count + 1;
			top += mFontHeight;
		}

		return MyGUI::IntPoint(left, top);
	}*/

} // namespace demo
