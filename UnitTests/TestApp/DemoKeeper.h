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

		//void addText(MyGUI::Widget* _parent);
		//void addImage(MyGUI::Widget* _parent);

		//void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void addLine(MyGUI::Widget* _parent, const std::string& _value);
		void parseText(MyGUI::Widget* _parent, const std::string& _value);
		void parseTag(MyGUI::Widget* _parent, const std::string& _value);

	private:
		bool mBold;
		bool mItalic;
		bool mStrike;
		bool mUnder;
		bool mImage;
		bool mHeader1;
		bool mHeader2;
		bool mHeader3;
		MyGUI::IntSize mImageSize;
		MyGUI::IntSize mSpacer;
		std::string mDefaultTextSkin;
		std::string mDefaultParagraphSkin;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
