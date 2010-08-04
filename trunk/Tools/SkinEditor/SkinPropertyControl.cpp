/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinPropertyControl.h"

namespace tools
{

	SkinPropertyControl::SkinPropertyControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("SkinPropertyControl.layout", _parent),
		mTextures(nullptr)
	{
		assignWidget(mTextures, "Textures");

		fillTextures();
	}

	SkinPropertyControl::~SkinPropertyControl()
	{
	}

	void SkinPropertyControl::fillTextures()
	{
		mTextures->removeAllItems();

		MyGUI::VectorString paths = MyGUI::DataManager::getInstance().getDataListNames("*.png");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
			mTextures->addItem(*iter);

		paths = MyGUI::DataManager::getInstance().getDataListNames("*.jpg");
		for (MyGUI::VectorString::iterator iter=paths.begin(); iter!=paths.end(); ++iter)
			mTextures->addItem(*iter);
	}
} // namespace tools
