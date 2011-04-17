/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#ifndef __MYGUI_HYPER_TEXT_BOX_H__
#define __MYGUI_HYPER_TEXT_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "StackPanel.h"
#include "ScrollViewPanel.h"

namespace MyGUI
{

	class HyperTextBox :
		public Widget
	{
		MYGUI_RTTI_DERIVED( HyperTextBox )

	public:
		HyperTextBox();

		void addItem(const std::string& _value);
		void updateContent();

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

	private:
		void addLine(Widget* _parent, const std::string& _value);
		void parseText(Widget* _parent, const std::string& _value);
		void parseTag(Widget* _parent, const std::string& _value);

		void OnTextLostFocus(Widget* _sender, Widget* _new);
		void OnTextSetFocus(Widget* _sender, Widget* _old);
		void OnTextClick(Widget* _sender);

	private:
		bool mBold;
		bool mItalic;
		bool mStrike;
		bool mUnder;
		bool mImage;
		bool mHeader1;
		bool mHeader2;
		bool mHeader3;
		bool mColour;
		bool mUrl;
		Colour mColourValue;
		IntSize mImageSize;
		IntSize mSpacer;
		std::string mDefaultTextSkin;
		std::string mDefaultParagraphSkin;
		std::string mUrlValue;

		StackPanel* mStackPanel;
		ScrollViewPanel* mScrollViewPanel;
	};

} // namespace MyGUI

#endif // __MYGUI_HYPER_TEXT_BOX_H__
