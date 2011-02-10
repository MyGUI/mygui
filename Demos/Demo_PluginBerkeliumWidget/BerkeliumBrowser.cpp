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
		mForwardButton(nullptr)
	{
		initialiseByAttributes(this, _parent);
	}

	BerkeliumBrowser::~BerkeliumBrowser()
	{
	}

} // demo
