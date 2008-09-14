/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "TextureView.h"

TextureView::TextureView() :
	BaseLayout("TextureView.layout")
{
}

void TextureView::initialise()
{
	loadLayout();
	assignWidget(mScrollView, "view_Texture");
	assignWidget(mImageBack, "image_Back");
	assignWidget(mImageTexture, "image_Texture");
}

void TextureView::update(const std::string & _font)
{
	MyGUI::FontManager & manager = MyGUI::FontManager::getInstance();
	if (!manager.isExist(_font)) return;

	MyGUI::FontPtr font = manager.getByName(_font);
	Ogre::TexturePtr texture = font->getTextureFont();
	if (texture.isNull()) return;

	const MyGUI::IntSize & size = MyGUI::SkinManager::getInstance().getTextureSize(texture->getName());

	mImageTexture->setImageTexture(texture->getName());
	mImageTexture->setPosition(0, 0, size.width, size.height);
	mImageBack->setPosition(0, 0, size.width, size.height);
	mScrollView->setCanvasSize(size);

}
