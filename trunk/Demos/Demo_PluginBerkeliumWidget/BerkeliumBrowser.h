#ifndef _BERKELIUM_BROWSER_H_
#define _BERKELIUM_BROWSER_H_

#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(BerkeliumBrowser, "BerkeliumBrowser.layout");
	class BerkeliumBrowser :
		public wraps::BaseLayout
	{
	public:
		BerkeliumBrowser(MyGUI::Widget* _parent = nullptr);
		virtual ~BerkeliumBrowser();

	private:
	//%LE Widget_Declaration list start
		ATTRIBUTE_FIELD_WIDGET_NAME(BerkeliumBrowser, mWebPageBerkeliumWidget, "WebPage");
		MyGUI::BerkeliumWidget* mWebPageBerkeliumWidget;
		ATTRIBUTE_FIELD_WIDGET_NAME(BerkeliumBrowser, mUrlEditBox, "Url");
		MyGUI::EditBox* mUrlEditBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(BerkeliumBrowser, mBackButton, "Back");
		MyGUI::Button* mBackButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(BerkeliumBrowser, mForwardButton, "Forward");
		MyGUI::Button* mForwardButton;
	//%LE Widget_Declaration list end
	};

} // namespace demo

#endif // _BERKELIUM_BROWSER_H_
