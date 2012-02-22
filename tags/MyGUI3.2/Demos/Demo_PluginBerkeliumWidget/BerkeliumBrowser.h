/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#ifndef _BERKELIUM_BROWSER_H_
#define _BERKELIUM_BROWSER_H_

#include "BaseLayout/BaseLayout.h"
#include "BerkeliumWidget.h"
#include "BerkeliumDelegate.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(BerkeliumBrowser, "BerkeliumBrowser.layout");
	class BerkeliumBrowser :
		public wraps::BaseLayout,
		public MyGUI::BerkeliumDelegate
	{
	public:
		BerkeliumBrowser(MyGUI::Widget* _parent = nullptr);
		virtual ~BerkeliumBrowser();

		void notifyEditSelectAccept(MyGUI::EditBox* _sender);
		void notifyBackButtonClick(MyGUI::Widget* _sender);
		void notifyForwardButtonClick(MyGUI::Widget* _sender);

	protected:
		// override Berkelium::WindowDelegate functions
		virtual void onAddressBarChanged(Berkelium::Window *win, Berkelium::URLString newURL);
		virtual void onLoadingStateChanged(Berkelium::Window *win, bool isLoading);
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
		ATTRIBUTE_FIELD_WIDGET_NAME(BerkeliumBrowser, mLoadingProgressBar, "Loading");
		MyGUI::ProgressBar* mLoadingProgressBar;
	//%LE Widget_Declaration list end
	};

} // namespace demo

#endif // _BERKELIUM_BROWSER_H_
