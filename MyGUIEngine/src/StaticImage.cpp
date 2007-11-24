
#include "StaticImage.h"
#include "SkinManager.h"

namespace MyGUI
{

	StaticImage::StaticImage(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name),
		mNum(0)
	{
		// первоначальная инициализация
		ASSERT(mSubSkinChild.size() == 1);
		mElement = static_cast<PanelAlphaOverlayElement *>(mSubSkinChild[0]->getOverlayElement());
		ASSERT(mElement);

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ImageMaterial");
			if (iter != param.end()) setImageMaterial(iter->second);
			iter = param.find("ImageRect");
			if (iter != param.end()) setImageRect(util::parseFloatRect(iter->second));
			iter = param.find("ImageTile");
			if (iter != param.end()) setImageTile(util::parseFloatSize(iter->second));
			iter = param.find("ImageNum");
			if (iter != param.end()) setImageNum(util::parseInt(iter->second));
		}
	}

	void StaticImage::setImageNum(size_t _num)
	{
		if (_num == mNum) return;
		mNum = _num;

		// если размер нулевой, то ставим размер текстуры
		if (!(mRectImage.right || mRectImage.bottom)) {
			mRectImage.right = mSizeTexture.width;
			mRectImage.bottom = mSizeTexture.width;
		}

		size_t count = (size_t)(mRectImage.getWidth() / mSizeTile.width);
		if (count < 1) count = 1;

		FloatRect offset(
			((float)(mNum % count)) * mSizeTile.width + mRectImage.left,
			((float)(mNum / count)) * mSizeTile.height + mRectImage.top,
			mSizeTile.width, mSizeTile.height);
		// конвертируем и устанавливаем
		offset = SkinManager::convertMaterialCoord(offset, mSizeTexture);
		mElement->setUV(offset.left, offset.top, offset.right, offset.bottom);
	}

	void StaticImage::setImageInfo(const std::string & _material, const FloatSize & _tile)
	{
		mElement->setMaterialName(_material);
		mSizeTexture = SkinManager::getMaterialSize(_material);
		mSizeTile = _tile;
		mNum = (size_t)-1;
	}

	void StaticImage::setImageInfo(const std::string & _material, const FloatRect & _rect, const FloatSize & _tile)
	{
		mElement->setMaterialName(_material);
		mSizeTexture = SkinManager::getMaterialSize(_material);
		mRectImage = _rect;
		mSizeTile = _tile;
		mNum = (size_t)-1;
	}

	void StaticImage::setImageMaterial(const std::string & _material)
	{
		mElement->setMaterialName(_material);
		mSizeTexture = SkinManager::getMaterialSize(_material);
		mNum = (size_t)-1;
	}

} // namespace MyGUI