/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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

	void TextureView::setFontName(const std::string& _value)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
		if (!manager.isExist(_value)) return;

		MyGUI::IFont* font = manager.getByName(_value)->castType<MyGUI::IFont>();
		MyGUI::ITexture* texture = font->getTextureFont();
		if (texture == nullptr) return;

		const MyGUI::IntSize& size = MyGUI::texture_utility::getTextureSize(texture->getName(), false);

		mImageTexture->setImageTexture(texture->getName());
		mImageTexture->setCoord(0, 0, size.width, size.height);
		mImageBack->setCoord(0, 0, size.width, size.height);
		mScrollView->setCanvasSize(size);
	}

} // namespace demo
