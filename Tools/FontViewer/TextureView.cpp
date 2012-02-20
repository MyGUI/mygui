/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "TextureView.h"

namespace demo
{

	TextureView::TextureView() :
		BaseLayout("TextureView.layout")
	{
		assignWidget(mWindow, "_Main");
		assignWidget(mScrollView, "view_Texture");
		assignWidget(mImageBack, "image_Back");
		assignWidget(mImageTexture, "image_Texture");
	}

	void TextureView::setFontName(const std::string& _value)
	{
		bool isOK = false;

		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();

		if (manager.isExist(_value))
		{
			MyGUI::IFont* font = manager.getByName(_value)->castType<MyGUI::IFont>();

			MyGUI::ITexture* texture = font->getTextureFont();

			if (texture != nullptr)
			{
				const MyGUI::IntSize& size = MyGUI::texture_utility::getTextureSize(texture->getName(), false);

				mWindow->setCaption(MyGUI::utility::toString("Texture View - ", size.width, " x ", size.height, " pixels"));

				mImageTexture->setImageTexture(texture->getName());
				mImageTexture->setCoord(0, 0, size.width, size.height);
				mImageBack->setCoord(0, 0, size.width, size.height);
				mScrollView->setCanvasSize(size);

				isOK = true;
			}
		}

		if (!isOK)
		{
			mWindow->setCaption("Texture View");

			mImageTexture->setImageTexture("");
			mImageTexture->setCoord(0, 0, 0, 0);
			mImageBack->setCoord(0, 0, 0, 0);
			mScrollView->setCanvasSize(0, 0);
		}
	}

} // namespace demo
