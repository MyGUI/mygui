/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_StaticImage.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	StaticImage::StaticImage(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mNum(ITEM_NONE)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(null != mMainSkin, "need one subskin");

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ImageTexture");
			if (iter != param.end()) setImageTexture(iter->second);
			iter = param.find("ImageRect");
			if (iter != param.end()) setImageRect(FloatRect::parse(iter->second));
			iter = param.find("ImageTile");
			if (iter != param.end()) setImageTile(FloatSize::parse(iter->second));
			iter = param.find("ImageNum");
			if (iter != param.end()) setImageNum(utility::parseInt(iter->second));
		}
	}

	void StaticImage::setImageNum(size_t _num)
	{
		if (_num == mNum) return;
		mNum = _num;

		if (_num == ITEM_NONE) {
			_setUVSet(FloatRect());
			return;
		}

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
		offset = SkinManager::convertTextureCoord(offset, mSizeTexture);
		_setUVSet(offset);
	}

	void StaticImage::setImageInfo(const std::string & _texture, const FloatSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mSizeTile = _tile;
		mNum = ITEM_NONE;
	}

	void StaticImage::setImageInfo(const std::string & _texture, const FloatRect & _rect, const FloatSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mRectImage = _rect;
		mSizeTile = _tile;
		mNum = ITEM_NONE;
	}

	void StaticImage::setImageTexture(const std::string & _texture)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mNum = ITEM_NONE;
	}

} // namespace MyGUI
