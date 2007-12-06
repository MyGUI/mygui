/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_StaticImage.h"
#include "MyGUI_SkinManager.h"
//#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	StaticImage::StaticImage(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_left, _top, _width, _height, _align, _info, _parent, _name),
		mNum(0)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(mSubSkinChild.size() == 1);
		MYGUI_DEBUG_ASSERT(false == mSubSkinChild[0]->_isText());

		mElement = static_cast<PanelAlphaOverlayElement *>(mSubSkinChild[0]->_getOverlayElement());
		MYGUI_DEBUG_ASSERT(null != mElement);

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ImageMaterial");
			if (iter != param.end()) setImageMaterial(iter->second);
			iter = param.find("ImageRect");
			if (iter != param.end()) setImageRect(FloatRect::parse(iter->second));
			iter = param.find("ImageTile");
			if (iter != param.end()) setImageTile(FloatSize::parse(iter->second));
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

		size_t count = (size_t)(mRectImage.width() / mSizeTile.width);
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