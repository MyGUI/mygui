#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{
	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

		void addLine(MyGUI::Widget* _parent, const std::string& _value);
		void parseText(MyGUI::Widget* _parent, const std::string& _value);
		void parseTag(MyGUI::Widget* _parent, const std::string& _value);

		void OnTextLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);
		void OnTextSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old);
		void OnTextClick(MyGUI::Widget* _sender);

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
		MyGUI::Colour mColourValue;
		MyGUI::IntSize mImageSize;
		MyGUI::IntSize mSpacer;
		std::string mDefaultTextSkin;
		std::string mDefaultParagraphSkin;
		std::string mUrlValue;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
