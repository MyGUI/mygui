/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "TextureViewToolControl.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TextureViewToolControl)

	TextureViewToolControl::TextureViewToolControl() :
		mTextureTool(nullptr),
		mPlace(nullptr)
	{
	}

	TextureViewToolControl::~TextureViewToolControl()
	{
		delete mTextureTool;
	}

	void TextureViewToolControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mPlace, "Place");

		mTextureTool = new SkinTextureControl(mPlace);
	}

}
