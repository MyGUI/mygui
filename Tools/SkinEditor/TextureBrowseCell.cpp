/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "precompiled.h"
#include "TextureBrowseCell.h"

namespace tools
{
	void TextureBrowseCell::getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop)
	{
		_coord.set(0, 0, 180, 180);
	}

	TextureBrowseCell::TextureBrowseCell(MyGUI::Widget* _parent) :
		wraps::BaseCellView<std::string>("TextureBrowseCell.layout", _parent)
	{
		assignWidget(mTextureName, "TextureName");
		assignWidget(mSelector, "Selector");
		assignWidget(mBack, "Back");
		assignWidget(mImage, "Image");
	}

	void TextureBrowseCell::update(const MyGUI::IBDrawItemInfo& _info, std::string _data)
	{
		if (_info.update)
		{
			mTextureName->setCaption(_data);
			const MyGUI::IntSize& textureSize = MyGUI::texture_utility::getTextureSize(_data);

			if (textureSize.width != 0 && textureSize.height != 0)
			{
				mBack->setVisible(true);
				mImage->setImageTexture(_data);

				const MyGUI::IntSize& targetSize = mSelector->getSize();
				//mBack->setSize();
			}
			else
			{
				mBack->setVisible(false);
			}
		}

		if (_info.select)
			mSelector->setAlpha(1);
		else
			mSelector->setAlpha(0);
	}

} // namespace tools
