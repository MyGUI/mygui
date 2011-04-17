/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#include "MyGUI_Precompiled.h"
#include <MyGUI_ResourceImageSet.h>
#include <MyGUI_ImageBox.h>
#include <MyGUI_TextBox.h>
#include "HyperTextBox.h"
#include "WrapPanel.h"

namespace MyGUI
{

	HyperTextBox::HyperTextBox() :
		mBold(false),
		mItalic(false),
		mStrike(false),
		mUnder(false),
		mImage(false),
		mHeader1(false),
		mHeader2(false),
		mHeader3(false),
		mColour(false),
		mUrl(false),
		mStackPanel(nullptr),
		mScrollViewPanel(nullptr)
	{
		mSpacer.set(3, 3);
		mDefaultTextSkin = "TextBox";
		mDefaultParagraphSkin = "Default";
		//mSpacer.set(10, 10);
		//mDefaultTextSkin = "Button";
		//mDefaultParagraphSkin = "Button";
	}

	void HyperTextBox::initialiseOverride()
	{
		Base::initialiseOverride();

		assignWidget(mScrollViewPanel, "ScrollViewPanel");
		assignWidget(mStackPanel, "StackPanel");

		//mScrollViewPanel = createWidget<ScrollViewPanel>("ScrollView", IntCoord(0, 0, getWidth(), getHeight()), Align::Stretch);
		//mStackPanel = mScrollViewPanel->createWidget<StackPanel>("PanelEmpty", IntCoord(), Align::Default);

		mStackPanel->setSpacer(IntSize(0, mSpacer.height));
		mScrollViewPanel->setCanvasAlign(Align::Default);
		mScrollViewPanel->setVisibleHScroll(false);
	}

	void HyperTextBox::shutdownOverride()
	{
		Base::shutdownOverride();
	}

	void HyperTextBox::addItem(const std::string& _value)
	{
		addLine(mStackPanel, _value);
	}

	void HyperTextBox::addLine(Widget* _parent, const std::string& _value)
	{
		WrapPanel* panel = _parent->createWidget<WrapPanel>(mDefaultParagraphSkin, IntCoord(), Align::Default);
		panel->setContentAlign(Align::Left | Align::Bottom);
		panel->setSpacer(IntSize(mSpacer.width, mSpacer.height));

		std::string::const_iterator textItem = _value.end();
		for (std::string::const_iterator item = _value.begin(); item != _value.end(); ++ item)
		{
			if ((*item) == '<')
			{
				// отрезаем текст
				if (textItem < item)
					parseText(panel, _value.substr(textItem - _value.begin(), item - textItem));

				// ищем конец тега
				for (std::string::const_iterator tagItem = item; tagItem != _value.end(); ++ tagItem)
				{
					if ((*tagItem) == '>')
					{
						parseTag(panel, _value.substr(item - _value.begin(), tagItem - item + 1));
						item = tagItem;
						textItem = tagItem + 1;
						break;
					}
				}
			}
		}
	}

	void HyperTextBox::parseText(Widget* _parent, const std::string& _value)
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

			ImageBox* image = _parent->createWidget<ImageBox>("ImageBox", IntCoord(0, 0, mImageSize.width, mImageSize.height), Align::Default);
			image->setItemResource(_value);
		}
		else if (mUrl)
		{
			TextBox* text = _parent->createWidget<TextBox>(mDefaultTextSkin, IntCoord(0, 0, defaultSize, defaultSize), Align::Default);
			text->setCaption(_value);
			text->setPointer("link");

			Widget* line = text->createWidget<Widget>("WhiteSkin", IntCoord(0, defaultSize - 1, defaultSize, 1), Align::HStretch | Align::Bottom);
			line->setColour(Colour::Black);
			line->setVisible(false);
			line->setNeedMouseFocus(false);

			text->eventMouseLostFocus += newDelegate(this, &HyperTextBox::OnTextLostFocus);
			text->eventMouseSetFocus += newDelegate(this, &HyperTextBox::OnTextSetFocus);
			text->eventMouseButtonClick += newDelegate(this, &HyperTextBox::OnTextClick);
			text->setUserString("Url", mUrlValue);
		}
		else
		{
			VectorString result = utility::split(_value);
			for (VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				TextBox* text = _parent->createWidget<TextBox>(mDefaultTextSkin, IntCoord(0, 0, defaultSize, defaultSize), Align::Default);
				text->setCaption(*item);
				if (mBold)
				{
					if (mItalic)
						text->setFontName("DejaVuSansBoldItalicFont.17");
					else
						text->setFontName("DejaVuSansBoldFont.17");
				}
				else if (mItalic)
				{
					text->setFontName("DejaVuSansItalicFont.17");
				}
				else if (mHeader1)
				{
					text->setFontName("DejaVuSansFont.26");
				}
				else if (mHeader2)
				{
					text->setFontName("DejaVuSansFont.25");
				}
				else if (mHeader3)
				{
					text->setFontName("DejaVuSansFont.20");
				}

				if (mColour)
				{
					text->setTextColour(mColourValue);
				}

				if (mStrike)
				{
					Widget* line = text->createWidget<Widget>("WhiteSkin", IntCoord(0, 0, defaultSize, 1), Align::HStretch | Align::VCenter);
					line->setColour(Colour::Black);
					line->setNeedMouseFocus(false);
					if (mColour)
						line->setColour(mColourValue);
				}

				if (mUnder)
				{
					Widget* line = text->createWidget<Widget>("WhiteSkin", IntCoord(0, defaultSize - 1, defaultSize, 1), Align::HStretch | Align::Bottom);
					line->setColour(Colour::Black);
					line->setNeedMouseFocus(false);
					if (mColour)
						line->setColour(mColourValue);
				}
			}
		}
	}

	void HyperTextBox::parseTag(Widget* _parent, const std::string& _value)
	{
		const std::string imageStartTagName = "<img";
		const std::string imageEndTagName = ">";

		const std::string paragraphStartTagName = "<p";
		const std::string paragraphEndTagName = ">";

		const std::string colourStartTagName = "<color";
		const std::string colourEndTagName = ">";

		const std::string urlStartTagName = "<url";
		const std::string urlEndTagName = ">";

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

			const std::string colourTagName = "value=";

			std::string valueColour = _value.substr(colourStartTagName.size(), _value.size() - (colourStartTagName.size() + colourEndTagName.size()));
			VectorString result = utility::split(valueColour);
			for (VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (utility::startWith(*item, colourTagName))
				{
					if ((colourTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(colourTagName.size() + 1, (*item).size() - (colourTagName.size() + 2));
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

			const std::string urlTagName = "value=";

			std::string valueUrl = _value.substr(urlStartTagName.size(), _value.size() - (urlStartTagName.size() + urlEndTagName.size()));
			VectorString result = utility::split(valueUrl);
			for (VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (utility::startWith(*item, urlTagName))
				{
					if ((urlTagName.size() + 2) < ((*item).size()))
					{
						mUrlValue = (*item).substr(urlTagName.size() + 1, (*item).size() - (urlTagName.size() + 2));
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
			Align alignResult = Align::Default;
			bool needAlign = false;

			Align floatResult = Align::Default;
			bool needFloat = false;

			const std::string alightTagName = "align=";
			const std::string floatTagName = "float=";

			std::string valueParagraph = _value.substr(paragraphStartTagName.size(), _value.size() - (paragraphStartTagName.size() + paragraphEndTagName.size()));
			VectorString result = utility::split(valueParagraph);
			for (VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (utility::startWith(*item, alightTagName))
				{
					if ((alightTagName.size() + 2) < ((*item).size()))
					{
						needAlign = true;
						std::string value = (*item).substr(alightTagName.size() + 1, (*item).size() - (alightTagName.size() + 2));
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
				else if (utility::startWith(*item, floatTagName))
				{
					if ((floatTagName.size() + 2) < ((*item).size()))
					{
						needFloat = true;
						std::string value = (*item).substr(floatTagName.size() + 1, (*item).size() - (floatTagName.size() + 2));
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

			WrapPanel* panel = _parent->castType<WrapPanel>(false);
			if (panel != nullptr)
			{
				if (needAlign)
				{
					panel->setContentAlign(alignResult);
				}
				if (needFloat)
				{
					panel->setContentFloat(true);
					panel->setSnapFloat(floatResult);
				}
			}
		}
		else if (_value == "</p>")
		{
		}
		else if (utility::startWith(_value, imageStartTagName))
		{
			mImageSize.clear();
			mImage = true;

			const std::string widthTagName = "width=";
			const std::string heightTagName = "height=";

			std::string valueImage = _value.substr(imageStartTagName.size(), _value.size() - (imageStartTagName.size() + imageEndTagName.size()));
			VectorString result = utility::split(valueImage);
			for (VectorString::const_iterator item = result.begin(); item != result.end(); ++ item)
			{
				if (utility::startWith(*item, widthTagName))
				{
					if ((widthTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(widthTagName.size() + 1, (*item).size() - (widthTagName.size() + 2));
						mImageSize.width = utility::parseValue<int>(value);
					}
				}
				else if (utility::startWith(*item, heightTagName))
				{
					if ((heightTagName.size() + 2) < ((*item).size()))
					{
						std::string value = (*item).substr(heightTagName.size() + 1, (*item).size() - (heightTagName.size() + 2));
						mImageSize.height = utility::parseValue<int>(value);
					}
				}
			}
		}
		else if (_value == "</img>")
		{
			mImage = false;
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
		std::string url = _sender->getUserString("Url");
		int test = 0;
	}

	void HyperTextBox::updateContent()
	{
		mScrollViewPanel->updateContent();
	}

} // namespace MyGUI
