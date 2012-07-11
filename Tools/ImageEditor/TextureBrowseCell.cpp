/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#include "Precompiled.h"
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
		assignWidget(mParentBack, "ParentBack");
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

				const MyGUI::IntSize& targetSize = mParentBack->getSize();

				float k1 = (float)targetSize.width / textureSize.width;
				float k2 = (float)targetSize.height / textureSize.height;
				float k = std::min( k1, k2);
				MyGUI::IntSize size = MyGUI::IntSize((int)(k *  (float)textureSize.width), (int)(k *  (float)textureSize.height));

				MyGUI::IntSize parentSize = mBack->getParent()->getSize();
				mBack->setCoord((parentSize.width - size.width) / 2, (parentSize.height - size.height) / 2, size.width, size.height);
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
