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

	Ogre::String StaticImage::WidgetTypeName = "StaticImage";

	StaticImage::StaticImage(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mNum(0)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(null != mMainSkin, "need one subskin");

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ImageTexture");
			if (iter != param.end()) setImageTexture(iter->second);
			iter = param.find("ImageRect");
			if (iter != param.end()) setImageRect(IntRect::parse(iter->second));
			iter = param.find("ImageTile");
			if (iter != param.end()) setImageTile(IntSize::parse(iter->second));
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
			mRectImage.right = mSizeTexture.width ? mSizeTexture.width : 1;
			mRectImage.bottom = mSizeTexture.height ? mSizeTexture.height : 1;
		}

		if (!(mSizeTile.width || mSizeTile.height)) {
			mSizeTile.width = mRectImage.width();
			mSizeTile.height = mRectImage.height();
		}

		size_t count = (size_t)(mRectImage.width() / mSizeTile.width);
		if (count < 1) count = 1;
		if ( (int)_num - 1 > (mRectImage.width() / mSizeTile.width) * (mRectImage.height() / mSizeTile.height))
			MYGUI_LOG(Warning, "StaticImage tile number " << _num << " out of range");

		FloatRect offset(
			((float)(mNum % count)) * mSizeTile.width + mRectImage.left,
			((float)(mNum / count)) * mSizeTile.height + mRectImage.top,
			mSizeTile.width, mSizeTile.height);
		// конвертируем и устанавливаем
		offset = SkinManager::convertTextureCoord(offset, IntSize(mSizeTexture.width, mSizeTexture.height));
		_setUVSet(offset);
	}

	void StaticImage::setImageInfo(const std::string & _texture, const IntSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mSizeTile = _tile;

		size_t num = mNum;
		mNum++;
		setImageNum(num);
	}

	void StaticImage::setImageInfo(const std::string & _texture, const IntRect & _rect, const IntSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mRectImage = _rect;
		mSizeTile = _tile;

		size_t num = mNum;
		mNum++;
		setImageNum(num);
	}

	void StaticImage::setImageTile(const IntSize & _tile)
	{
		mSizeTile = _tile;

		size_t num = mNum;
		mNum++;
		setImageNum(num);
	}

	void StaticImage::setImageRect(const IntRect & _rect)
	{
		mRectImage = _rect;

		size_t num = mNum;
		mNum++;
		setImageNum(num);
	}

	void StaticImage::setImageTexture(const std::string & _texture)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);

		size_t num = mNum;
		mNum++;
		setImageNum(num);
	}

} // namespace MyGUI
