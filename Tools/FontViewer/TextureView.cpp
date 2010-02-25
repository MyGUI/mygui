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
		assignWidget(mImageTexture, "image_Texture");
	}

	void TextureView::setFontName(const std::string& _value)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
		if (!manager.isExist(_value))
		{
			mImageTexture->setImageTexture("");
			return;
		}

		MyGUI::IFont* font = manager.getByName(_value)->castType<MyGUI::IFont>();
		MyGUI::ITexture* texture = font->getTextureFont();
		if (texture == nullptr)
		{
			mImageTexture->setImageTexture("");
			return;
		}

		mImageTexture->setImageTexture(texture->getName());
	}

} // namespace demo
