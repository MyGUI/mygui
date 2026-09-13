/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#ifndef MYGUI_HYPER_TEXT_BOX_H_
#define MYGUI_HYPER_TEXT_BOX_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"
#include "WrapPanel.h"

namespace MyGUI
{

	class HyperTextBox : public Widget
	{
		MYGUI_RTTI_DERIVED(HyperTextBox)

	public:
		HyperTextBox();

		void setCaption(const UString& _value);

		void addItem(std::string_view _value);

		size_t getItemsCount() const;

		void removeItemAt(size_t _index);

		void removeAllItems();

		void updateContent();

		const IntSize& getSpacer() const;
		void setSpacer(const IntSize& _value);

		const std::string& getHeader1Font() const;
		void setHeader1Font(std::string_view _value);

		const std::string& getHeader2Font() const;
		void setHeader2Font(std::string_view _value);

		const std::string& getHeader3Font() const;
		void setHeader3Font(std::string_view _value);

		const std::string& getDefaultFont() const;
		void setDefaultFont(std::string_view _value);

		const std::string& getBoldFont() const;
		void setBoldFont(std::string_view _value);

		const std::string& getItalicFont() const;
		void setItalicFont(std::string_view _value);

		const std::string& getBoldItalicFont() const;
		void setBoldItalicFont(std::string_view _value);

		/*events:*/
		/** Event : Click to Url.\n
			signature : void method(MyGUI::HyperTextBox* _sender, std::string_view _url)
			@param _sender widget that called this event
			@param _url
		*/
		delegates::MultiDelegate<HyperTextBox*, std::string_view> eventUrlClick;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void setPropertyOverride(std::string_view _key, std::string_view _value) override;

	private:
		void parseParagraph(Widget* _parent, std::string_view _value);
		void parseText(Widget* _parent, std::string_view _value);
		void parseTag(std::string_view _value);

		void OnTextLostFocus(Widget* _sender, Widget* _new);
		void OnTextSetFocus(Widget* _sender, Widget* _old);
		void OnTextClick(Widget* _sender);

		void updateBreakHeight();

	private:
		bool mBold{false};
		bool mItalic{false};
		bool mStrike{false};
		bool mUnder{false};
		bool mImage{false};
		bool mHeader1{false};
		bool mHeader2{false};
		bool mHeader3{false};
		bool mColour{false};
		bool mUrl{false};
		Colour mColourValue;
		IntSize mImageSize;
		IntSize mSpacer;
		std::string mTextSkin;
		std::string mParagraphSkin;
		std::string mImageSkin;
		std::string mLineSkin;
		std::string mUrlValue;
		std::string mLinkPoiner;

		std::string mHeader1Font;
		std::string mHeader2Font;
		std::string mHeader3Font;
		std::string mDefaultFont;
		std::string mBoldFont;
		std::string mItalicFont;
		std::string mBoldItalicFont;

		StackPanel* mStackPanel{nullptr};
		ScrollViewPanel* mScrollViewPanel{nullptr};
		WrapPanel* mCurrentWrapPanel{nullptr};

		int mBreakHeight{0};
	};

} // namespace MyGUI

#endif // MYGUI_HYPER_TEXT_BOX_H_
