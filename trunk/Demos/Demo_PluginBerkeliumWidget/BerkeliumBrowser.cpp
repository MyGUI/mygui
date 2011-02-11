/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#include "Precompiled.h"
#include "BerkeliumBrowser.h"

namespace demo
{

	BerkeliumBrowser::BerkeliumBrowser(MyGUI::Widget* _parent) :
		mWebPageBerkeliumWidget(nullptr),
		mUrlEditBox(nullptr),
		mBackButton(nullptr),
		mForwardButton(nullptr),
		mLoadingProgressBar(nullptr)
	{
		initialiseByAttributes(this, _parent);

		mWebPageBerkeliumWidget->getBerkeliumWindow()->setTransparent(true);
		mUrlEditBox->eventEditSelectAccept += MyGUI::newDelegate(this, &BerkeliumBrowser::notifyEditSelectAccept);
		mBackButton->eventMouseButtonClick += MyGUI::newDelegate(this, &BerkeliumBrowser::notifyBackButtonClick);
		mForwardButton->eventMouseButtonClick += MyGUI::newDelegate(this, &BerkeliumBrowser::notifyForwardButtonClick);

		mWebPageBerkeliumWidget->setWindowDelegate(this);
	}

	BerkeliumBrowser::~BerkeliumBrowser()
	{
	}

	void BerkeliumBrowser::notifyEditSelectAccept(MyGUI::EditBox* _sender)
	{
		mWebPageBerkeliumWidget->loadURL(_sender->getOnlyText());
	}

	void BerkeliumBrowser::notifyBackButtonClick(MyGUI::Widget* _sender)
	{
		mWebPageBerkeliumWidget->getBerkeliumWindow()->goBack();
	}

	void BerkeliumBrowser::notifyForwardButtonClick(MyGUI::Widget* _sender)
	{
		mWebPageBerkeliumWidget->getBerkeliumWindow()->goForward();
	}

	void BerkeliumBrowser::onAddressBarChanged(Berkelium::Window *win, Berkelium::URLString newURL)
	{
		mUrlEditBox->setCaption(newURL.data());
		mBackButton->setEnabled(mWebPageBerkeliumWidget->getBerkeliumWindow()->canGoBack());
		mForwardButton->setEnabled(mWebPageBerkeliumWidget->getBerkeliumWindow()->canGoForward());
	}

	void BerkeliumBrowser::onLoadingStateChanged(Berkelium::Window *win, bool isLoading)
	{
		mLoadingProgressBar->setVisible(isLoading);
	}

} // demo
