/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#include "MyGUI_Precompiled.h"
#include <MyGUI_ResourceImageSet.h>
#include <MyGUI_ImageBox.h>
#include <MyGUI_TextBox.h>
#include <MyGUI_WidgetManager.h>
#include <MyGUI_FontManager.h>
#include <MyGUI_IFont.h>
#include "HyperTextBox.h"

namespace MyGUI
{

	HyperTextBox::HyperTextBox() :
		mTextSkin("TextBox"),
		mImageSkin("ImageBox"),
		mParagraphSkin("Default"),
		mLineSkin("WhiteSkin"),
		mLinkPoiner("link"),
		mHeader1Font("Default"),
		mHeader2Font("Default"),
		mHeader3Font("Default"),
		mDefaultFont("Default"),
		mBoldFont("Default"),
		mItalicFont("Default"),
		mBoldItalicFont("Default")
	{
	}

	void HyperTextBox::initialiseOverride()
	{
		Base::initialiseOverride();

		assignWidget(mScrollViewPanel, "ScrollViewPanel");
		assignWidget(mStackPanel, "StackPanel");

		if (isUserString("TextSkin"))
			mTextSkin = getUserString("TextSkin");
		if (isUserString("ImageSkin"))
			mImageSkin = getUserString("ImageSkin");
		if (isUserString("ParagraphSkin"))
			mParagraphSkin = getUserString("ParagraphSkin");
		if (isUserString("LineSkin"))
			mLineSkin = getUserString("LineSkin");
		if (isUserString("LinkPointer"))
			mLinkPoiner = getUserString("LinkPointer");

		updateBreakHeight();
	}

	void HyperTextBox::shutdownOverride()
	{
		Base::shutdownOverride();
	}

	void HyperTextBox::addItem(std::string_view _value)
	{
		parseParagraph(mStackPanel, _value);
		mCurrentWrapPanel = nullptr;
	}

	void HyperTextBox::parseParagraph(Widget* _parent, std::string_view _value)
	{
		auto textItem = _value.end();
		for (auto item = _value.begin(); item != _value.end(); ++item)
		{
			if ((*item) == '<')
			{
				// отрезаем текст
				if (textItem < item)
				{
					if (mCurrentWrapPanel != nullptr)
						parseText(mCurrentWrapPanel, _value.substr(textItem - _value.begin(), item - textItem));
				}

				// ищем конец тега
				for (auto tagItem = item; tagItem != _value.end(); ++tagItem)
				{
					if ((*tagItem) == '>')
					{
						parseTag(_value.substr(item - _value.begin(), tagItem - item + 1));

						item = tagItem;
						textItem = tagItem + 1;
						break;
					}
				}
			}
		}
	}

	void HyperTextBox::parseText(Widget* _parent, std::string_view _value)
	{
		const int defaultSize = 16;

		if (mImage)
		{
			if (mImageSize.width == 0 || mImageSize.height == 0)
			{
				IResource* resource = ResourceManager::getInstance().getByName(_value, false);
				if (resource != nullptr)
				{
					ResourceImageSet* imageSet = resource->castType<ResourceImageSet>(false);
					if (imageSet != nullptr)
					{
						ImageIndexInfo info = imageSet->getIndexInfo(0, 0);
						mImageSize = info.size;
					}
				}
			}

			ImageBox* image = _parent->createWidget<ImageBox>(
				mImageSkin,
				IntCoord(0, 0, mImageSize.width, mImageSize.height),
				Align::Default);
			image->setItemResource(_value);
			// картинка как урл
			if (mUrl)
			{
				image->setPointer(mLinkPoiner);
				image->eventMouseButtonClick += newDelegate(this, &HyperTextBox::OnTextClick);
				image->setUserString("URL", mUrlValue);
			}
		}
		else if (mUrl)
		{
			TextBox* text =
				_parent->createWidget<TextBox>(mTextSkin, IntCoord(0, 0, defaultSize, defaultSize), Align::Default);
			text->setCaption(MyGUI::UString(_value));
			text->setPointer(mLinkPoiner);
			if (mBold)
			{
				if (mItalic)
					text->setFontName(mBoldItalicFont);
				else
					text->setFontName(mBoldFont);
			}
			else if (mItalic)
			{
				text->setFontName(mItalicFont);
			}

			Widget* line = text->createWidget<Widget>(
				mLineSkin,
				IntCoord(0, defaultSize - 1, defaultSize, 1),
				Align::HStretch | Align::Bottom);
			line->setColour(Colour::Black);
			line->setVisible(false);
			line->setNeedMouseFocus(false);

			text->eventMouseLostFocus += newDelegate(this, &HyperTextBox::OnTextLostFocus);
			text->eventMouseSetFocus += newDelegate(this, &HyperTextBox::OnTextSetFocus);
			text->eventMouseButtonClick += newDelegate(this, &HyperTextBox::OnTextClick);
			text->setUserString("URL", mUrlValue);
		}
		else
		{
			VectorString result = utility::split(_value);
			for (const auto& item : result)
			{
				TextBox* text =
					_parent->createWidget<TextBox>(mTextSkin, IntCoord(0, 0, defaultSize, defaultSize), Align::Default);
				text->setCaption(item);
				if (mBold)
				{
					if (mItalic)
						text->setFontName(mBoldItalicFont);
					else
						text->setFontName(mBoldFont);
				}
				else if (mItalic)
				{
					text->setFontName(mItalicFont);
				}
				else if (mHeader1)
				{
					text->setFontName(mHeader1Font);
				}
				else if (mHeader2)
				{
					text->setFontName(mHeader2Font);
				}
				else if (mHeader3)
				{
					text->setFontName(mHeader3Font);
				}

				if (mColour)
				{
					text->setTextColour(mColourValue);
				}

				if (mStrike)
				{
					Widget* line = text->createWidget<Widget>(
						mLineSkin,
						IntCoord(0, 0, defaultSize, 1),
						Align::HStretch | Align::VCenter);
					line->setColour(Colour::Black);
					line->setNeedMouseFocus(false);
					if (mColour)
						line->setColour(mColourValue);
				}

				if (mUnder)
				{
					Widget* line = text->createWidget<Widget>(
						mLineSkin,
						IntCoord(0, defaultSize - 1, defaultSize, 1),
						Align::HStretch | Align::Bottom);
					line->setColour(Colour::Black);
					line->setNeedMouseFocus(false);
					if (mColour)
						line->setColour(mColourValue);
				}
			}
		}
	}

	void HyperTextBox::parseTag(std::string_view _value)
	{
		const std::string_view imageStartTagName = "<img";
		const std::string_view imageEndTagName = ">";

		const std::string_view paragraphStartTagName = "<p";
		const std::string_view paragraphEndTagName = ">";

		const std::string_view colourStartTagName = "<color";
		const std::string_view colourEndTagName = ">";

		const std::string_view urlStartTagName = "<url";
		const std::string_view urlEndTagName = ">";

		if (_value == "<h1>")
		{
			mHeader1 = true;
		}
		else if (_value == "</h1>")
		{
			mHeader1 = false;
		}
		else if (_value == "<h2>")
		{
			mHeader2 = true;
		}
		else if (_value == "</h2>")
		{
			mHeader2 = false;
		}
		else if (_value == "<h3>")
		{
			mHeader3 = true;
		}
		else if (_value == "</h3>")
		{
			mHeader3 = false;
		}
		else if (_value == "<b>")
		{
			mBold = true;
		}
		else if (_value == "</b>")
		{
			mBold = false;
		}
		else if (_value == "<i>")
		{
			mItalic = true;
		}
		else if (_value == "</i>")
		{
			mItalic = false;
		}
		else if (_value == "<s>")
		{
			mStrike = true;
		}
		else if (_value == "</s>")
		{
			mStrike = false;
		}
		else if (_value == "<u>")
		{
			mUnder = true;
		}
		else if (_value == "</u>")
		{
			mUnder = false;
		}
		else if (utility::startWith(_value, colourStartTagName))
		{
			mColour = true;
			mColourValue = Colour::Black;

			const std::string_view colourTagName = "value=";

			std::string_view valueColour = _value.substr(
				colourStartTagName.size(),
				_value.size() - (colourStartTagName.size() + colourEndTagName.size()));
			VectorString result = utility::split(valueColour);
			for (const auto& item : result)
			{
				if (utility::startWith(item, colourTagName))
				{
					if ((colourTagName.size() + 2) < (item.size()))
					{
						std::string_view value = std::string_view(item).substr(
							colourTagName.size() + 1,
							item.size() - (colourTagName.size() + 2));
						mColourValue = Colour::parse(value);
					}
				}
			}
		}
		else if (_value == "</color>")
		{
			mColour = false;
		}
		else if (utility::startWith(_value, urlStartTagName))
		{
			mUrl = true;
			mUrlValue.clear();

			const std::string_view urlTagName = "value=";

			std::string_view valueUrl =
				_value.substr(urlStartTagName.size(), _value.size() - (urlStartTagName.size() + urlEndTagName.size()));
			VectorString result = utility::split(valueUrl);
			for (const auto& item : result)
			{
				if (utility::startWith(item, urlTagName))
				{
					if ((urlTagName.size() + 2) < (item.size()))
					{
						mUrlValue = item.substr(urlTagName.size() + 1, item.size() - (urlTagName.size() + 2));
					}
				}
			}
		}
		else if (_value == "</url>")
		{
			mUrl = false;
		}
		else if (utility::startWith(_value, paragraphStartTagName))
		{
			mCurrentWrapPanel = mStackPanel->createWidget<WrapPanel>(mParagraphSkin, IntCoord(), Align::Default);
			mCurrentWrapPanel->setContentAlign(Align::Left | Align::Bottom);
			mCurrentWrapPanel->setSpacer(mSpacer);

			Align alignResult = Align::Default;
			bool needAlign = false;

			Align floatResult = Align::Default;
			bool needFloat = false;

			const std::string_view alightTagName = "align=";
			const std::string_view floatTagName = "float=";

			std::string_view valueParagraph = _value.substr(
				paragraphStartTagName.size(),
				_value.size() - (paragraphStartTagName.size() + paragraphEndTagName.size()));
			VectorString result = utility::split(valueParagraph);
			for (const auto& item : result)
			{
				if (utility::startWith(item, alightTagName))
				{
					if ((alightTagName.size() + 2) < (item.size()))
					{
						needAlign = true;
						std::string_view value = std::string_view(item).substr(
							alightTagName.size() + 1,
							item.size() - (alightTagName.size() + 2));
						if (value == "left")
							alignResult = Align::Default;
						else if (value == "center")
							alignResult = Align::HCenter | Align::Top;
						else if (value == "right")
							alignResult = Align::Right | Align::Top;
						else
							alignResult = Align::Default;
					}
				}
				else if (utility::startWith(item, floatTagName))
				{
					if ((floatTagName.size() + 2) < (item.size()))
					{
						needFloat = true;
						std::string_view value = std::string_view(item).substr(
							floatTagName.size() + 1,
							item.size() - (floatTagName.size() + 2));
						if (value == "left")
							floatResult = Align::Default;
						else if (value == "center")
							floatResult = Align::HCenter | Align::Top;
						else if (value == "right")
							floatResult = Align::Right | Align::Top;
						else
							floatResult = Align::Default;
					}
				}
			}

			if (needAlign)
			{
				mCurrentWrapPanel->setContentAlign(alignResult);
			}
			if (needFloat)
			{
				mCurrentWrapPanel->setContentFloat(true);
				mCurrentWrapPanel->setSnapFloat(floatResult);
			}
		}
		else if (_value == "</p>")
		{
			mCurrentWrapPanel = nullptr;
		}
		else if (utility::startWith(_value, imageStartTagName))
		{
			mImageSize.clear();
			mImage = true;

			const std::string_view widthTagName = "width=";
			const std::string_view heightTagName = "height=";

			std::string_view valueImage = _value.substr(
				imageStartTagName.size(),
				_value.size() - (imageStartTagName.size() + imageEndTagName.size()));
			VectorString result = utility::split(valueImage);
			for (const auto& item : result)
			{
				if (utility::startWith(item, widthTagName))
				{
					if ((widthTagName.size() + 2) < (item.size()))
					{
						std::string_view value = std::string_view(item).substr(
							widthTagName.size() + 1,
							item.size() - (widthTagName.size() + 2));
						mImageSize.width = utility::parseValue<int>(value);
					}
				}
				else if (utility::startWith(item, heightTagName))
				{
					if ((heightTagName.size() + 2) < (item.size()))
					{
						std::string_view value = std::string_view(item).substr(
							heightTagName.size() + 1,
							item.size() - (heightTagName.size() + 2));
						mImageSize.height = utility::parseValue<int>(value);
					}
				}
			}
		}
		else if (_value == "</img>")
		{
			mImage = false;
		}
		else if (_value == "<br/>")
		{
			if (mCurrentWrapPanel == nullptr)
			{
				WrapPanel* panel = mStackPanel->createWidget<WrapPanel>(mParagraphSkin, IntCoord(), Align::Default);
				panel->setContentAlign(Align::Left | Align::Bottom);
				panel->setSpacer(mSpacer);
				panel->createWidget<Widget>("Default", IntCoord(0, 0, 0, mBreakHeight), Align::Default);
			}
		}
		else if (_value == "<t/>")
		{
			if (mCurrentWrapPanel != nullptr)
			{
				const int countInTab = 4;
				mCurrentWrapPanel->createWidget<Widget>(
					"Default",
					IntCoord(0, 0, countInTab * mSpacer.width, 0),
					Align::Default);
			}
		}
	}

	void HyperTextBox::OnTextLostFocus(Widget* _sender, Widget* _new)
	{
		size_t count = _sender->getChildCount();
		if (count > 0)
			_sender->getChildAt(0)->setVisible(false);
	}

	void HyperTextBox::OnTextSetFocus(Widget* _sender, Widget* _old)
	{
		size_t count = _sender->getChildCount();
		if (count > 0)
			_sender->getChildAt(0)->setVisible(true);
	}

	void HyperTextBox::OnTextClick(Widget* _sender)
	{
		std::string_view url = _sender->getUserString("URL");
		eventUrlClick(this, url);
	}

	void HyperTextBox::updateContent()
	{
		mStackPanel->setSpacer(IntSize(0, mSpacer.height));
		mScrollViewPanel->updateContent();
	}

	const IntSize& HyperTextBox::getSpacer() const
	{
		return mSpacer;
	}

	void HyperTextBox::setSpacer(const IntSize& _value)
	{
		mSpacer = _value;
	}

	const std::string& HyperTextBox::getHeader1Font() const
	{
		return mHeader1Font;
	}

	void HyperTextBox::setHeader1Font(std::string_view _value)
	{
		mHeader1Font = _value;
	}

	const std::string& HyperTextBox::getHeader2Font() const
	{
		return mHeader2Font;
	}

	void HyperTextBox::setHeader2Font(std::string_view _value)
	{
		mHeader2Font = _value;
	}

	const std::string& HyperTextBox::getHeader3Font() const
	{
		return mHeader3Font;
	}

	void HyperTextBox::setHeader3Font(std::string_view _value)
	{
		mHeader3Font = _value;
	}

	const std::string& HyperTextBox::getDefaultFont() const
	{
		return mDefaultFont;
	}

	void HyperTextBox::setDefaultFont(std::string_view _value)
	{
		mDefaultFont = _value;
		updateBreakHeight();
	}

	const std::string& HyperTextBox::getBoldFont() const
	{
		return mBoldFont;
	}

	void HyperTextBox::setBoldFont(std::string_view _value)
	{
		mBoldFont = _value;
	}

	const std::string& HyperTextBox::getItalicFont() const
	{
		return mItalicFont;
	}

	void HyperTextBox::setItalicFont(std::string_view _value)
	{
		mItalicFont = _value;
	}

	const std::string& HyperTextBox::getBoldItalicFont() const
	{
		return mBoldItalicFont;
	}

	void HyperTextBox::setBoldItalicFont(std::string_view _value)
	{
		mBoldItalicFont = _value;
	}

	void HyperTextBox::setPropertyOverride(std::string_view _key, std::string_view _value)
	{
		if (_key == "Spacer")
			setSpacer(utility::parseValue<IntSize>(_value));
		else if (_key == "DefaultFont")
			setDefaultFont(_value);
		else if (_key == "BoldFont")
			setBoldFont(_value);
		else if (_key == "ItalicFont")
			setItalicFont(_value);
		else if (_key == "BoldItalicFont")
			setBoldItalicFont(_value);
		else if (_key == "Header1Font")
			setHeader1Font(_value);
		else if (_key == "Header2Font")
			setHeader2Font(_value);
		else if (_key == "Header3Font")
			setHeader3Font(_value);
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	void HyperTextBox::setCaption(const UString& _value)
	{
		removeAllItems();

		addItem(_value);

		updateContent();
	}

	void HyperTextBox::removeAllItems()
	{
		while (mStackPanel->getChildCount() != 0)
			WidgetManager::getInstancePtr()->destroyWidget(mStackPanel->getChildAt(0));
	}

	size_t HyperTextBox::getItemsCount() const
	{
		return mStackPanel->getChildCount();
	}

	void HyperTextBox::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mStackPanel->getChildCount(), "HyperTextBox::removeItemAt");
		WidgetManager::getInstancePtr()->destroyWidget(mStackPanel->getChildAt(_index));
	}

	void HyperTextBox::updateBreakHeight()
	{
		mBreakHeight = 0;

		IFont* font = FontManager::getInstancePtr()->getByName(getDefaultFont());
		if (font != nullptr)
			mBreakHeight = font->getDefaultHeight();
	}

} // namespace MyGUI
