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

	TextureViewToolControl::TextureViewToolControl()
	{
	}

	TextureViewToolControl::~TextureViewToolControl()
	{
	}

	void TextureViewToolControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);
	}

}
