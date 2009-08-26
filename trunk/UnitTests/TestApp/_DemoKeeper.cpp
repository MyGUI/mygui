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

	// массив для быстрой конвертации цветов
	static const char convert_colour[64] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0 };

	MyGUI::IntSize DemoKeeper::getSizeTextNativeHeight(const MyGUI::UString& _text, MyGUI::FontPtr _font)
	{
		MyGUI::IntSize result;
		int height = _font->getHeightPix();
		int width = 0;
		size_t count = 0;
		mLength = 0;
		MyGUI::UString::const_iterator end = _text.end();
		MyGUI::UString::const_iterator index = _text.begin();

		mLineInfo.clear();
		LineInfo line_info;

		if (index == end)
			return result;

		result.height += height;

		for (; index!=end; ++index)
		{
			MyGUI::Char character = *index;

			// новая строка
			if (character == MyGUI::Font::FONT_CODE_CR
				|| character == MyGUI::Font::FONT_CODE_NEL
				|| character == MyGUI::Font::FONT_CODE_LF)
			{

				if (character == MyGUI::Font::FONT_CODE_CR)
				{
					MyGUI::UString::const_iterator peeki = index;
					peeki ++;
					if ((peeki != end) && (*peeki == MyGUI::Font::FONT_CODE_LF))
						index = peeki; // skip both as one newline
				}

				line_info.width = width;
				line_info.count = count;
				line_info.newline = true;
				mLength += line_info.count + 1;

				result.height += height;
				if (result.width < width)
					result.width = width;
				width = 0;
				count = 0;

				mLineInfo.push_back(line_info);
				line_info.clear();

				continue;
			}
			// тег
			else if (character == L'#')
			{
				// берем следующий символ
				++ index;
				if (index == end) { --index ;continue; } // это защита

				character = *index;
				// если два подряд, то рисуем один шарп, если нет то меняем цвет
				if (character != L'#')
				{
					// парсим первый символ
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// и еще пять символов после шарпа
					for (char i=0; i<5; i++)
					{
						++ index;
						if (index == end) { --index ;continue; } // это защита
						colour <<= 4;
						colour += convert_colour[ ((*index) - 48) & 0x3F];
					}

					// если нужно, то меняем красный и синий компоненты
					//MYGUI_CONVERT_COLOUR(colour, mRenderGL);

					// запоминаем цвет, в верхнем байте единицы
					//mLinesInfo.back().second.push_back( EnumCharInfo(colour, true) );

					continue;
				}
			}

			MyGUI::Font::GlyphInfo* info = 0;
			if (MyGUI::Font::FONT_CODE_SPACE == character)
			{
				info = _font->getSpaceGlyphInfo();
			}
			else if (MyGUI::Font::FONT_CODE_TAB == character)
			{
				info = _font->getTabGlyphInfo();
			}
			else
			{
				info = _font->getGlyphInfo(character);
			}

			line_info.simbols.push_back(info->mPixSize.width);
			width += info->mPixSize.width;
			count ++;
		}

		line_info.width = width;
		line_info.count = count;
		mLength += line_info.count;

		mLineInfo.push_back(line_info);

		if (result.width < width)
			result.width = width;

		return result;
	}

	MyGUI::IntSize DemoKeeper::getSizeTextScaleHeight(const MyGUI::UString& _text, MyGUI::FontPtr font, int _height)
	{
		return MyGUI::IntSize();
	}

	MyGUI::IntSize DemoKeeper::getSizeText(const MyGUI::UString& _text, const std::string& _fontname, int _height)
	{
		MyGUI::FontPtr font = MyGUI::FontManager::getInstance().getByName(_fontname);
		int height = font->getHeightPix();
		if (height == _height) return getSizeTextNativeHeight(_text, font);
		return getSizeTextScaleHeight(_text, font, _height);
	}

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

		const std::string test = "test\ntest\ntest";
		const MyGUI::IntSize& size = getSizeText(test, "Default", 17);
		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(80, 80, size.width, size.height), MyGUI::Align::Default, "Overlapped");
		mText = widget->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Stretch);
		mText->setCaption(test);
		mText->setTextColour(MyGUI::Colour::White);

		mCursor = mGUI->createWidget<MyGUI::StaticText>("MarkerGreen", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::Align::Default, "Statistic");

		MyGUI::MYGUI_OUT(size.print());

		mCursorPosition = 0;
		setCursorPosition(mCursorPosition);
	}

	void DemoKeeper::destroyScene()
	{
		mView->getPanelView()->removeAllItems();
		delete mView;

		delete mPanelDirector;
		delete mPanelDynamic;
		delete mPanelStatic;
	}

	void DemoKeeper::notifyChangePanels(int _key, size_t _value)
	{
		if (_key == EVENT_SHOW_STATIC) {
			mView->getPanelView()->setItemShow(mPanelStatic, _value != 0);
		}
		else if (_key == EVENT_SHOW_DYNAMIC) {
			mView->getPanelView()->setItemShow(mPanelDynamic, _value != 0);
		}
		else if (_key == EVENT_COUNT_DYNAMIC) {
			mPanelDynamic->setVisibleCount(_value);
		}
	}

	bool DemoKeeper::mouseMoved( const OIS::MouseEvent &arg )
	{
		mCursorPosition = getCursorPosition(arg.state.X.abs - mText->getAbsoluteLeft(), arg.state.Y.abs - mText->getAbsoluteTop());
		setCursorPosition(mCursorPosition);
		return base::BaseManager::mouseMoved(arg);
	}

	size_t DemoKeeper::getCursorPosition(int _left, int _top)
	{
		const int height = 17;
		size_t result = 0;
		int top = 0;

		for (VectorLineInfo::const_iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
		{
			if (top + (height / 2) > _top)
			{
				top += height;
				break;
			}

			top += height;
			result += line->count;
			result += line->newline ? 1 : 0;

			//if (position + line->count >= _position)
			{
				//for (std::vector<int>::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
				{
					/*if (position == _position)
					{
						break;
					}

					position ++;
					left += *sim;*/
				}
				//break;*/
			}
			/*position += line->count;
			position += line->newline ? 1 : 0;
			top += 17;*/
		}

		return result;
	}

	void DemoKeeper::setCursorPosition(size_t _position)
	{
		if (_position >= mLength + 1) _position = mLength;

		size_t position = 0;
		int top = 0;
		int left = 0;
		for (VectorLineInfo::const_iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
		{
			if (position + line->count >= _position)
			{
				for (std::vector<int>::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
				{
					if (position == _position)
					{
						break;
					}

					position ++;
					left += *sim;
				}
				break;
			}
			position += line->count;
			position += line->newline ? 1 : 0;
			top += 17;
		}

		mCursor->setCoord(left + mText->getAbsoluteLeft(), top + mText->getAbsoluteTop(), 2, 17);
	}

} // namespace demo
