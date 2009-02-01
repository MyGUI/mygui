/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "TextureView.h"

namespace demo
{

	TextureView::TextureView() : BaseLayout("TextureView.layout")
	{
		assignWidget(mScrollView, "view_Texture");
		assignWidget(mImageBack, "image_Back");
		assignWidget(mImageTexture, "image_Texture");
	}

	void TextureView::setFontName(const std::string & _font)
	{
		MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
		if (!manager.isExist(_font)) return;

		MyGUI::FontPtr font = manager.getByName(_font);
		Ogre::TexturePtr texture = font->getTextureFont();
		if (texture.isNull()) return;

		const MyGUI::IntSize & size = MyGUI::SkinManager::getInstance().getTextureSize(texture->getName());

		mImageTexture->setImageTexture(texture->getName());
		mImageTexture->setCoord(0, 0, size.width, size.height);
		mImageBack->setCoord(0, 0, size.width, size.height);
		mScrollView->setCanvasSize(size);

	}

} // namespace demo
