/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_StaticImage.h"
#include "MyGUI_SkinManager.h"
//#include "MyGUI_SharedPanelAlphaOverlayElement.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	StaticImage::StaticImage(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mNum(ITEM_NONE)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(mSubSkinChild.size() == 1, "subskin must be one");
		MYGUI_DEBUG_ASSERT(false == mSubSkinChild[0]->_isText(), "subskin must be not text");
		mElementSkin = mSubSkinChild.front();

		//mElement = static_cast<PanelAlphaOverlayElement *>(mSubSkinChild[0]->_getOverlayElement());
		//MYGUI_DEBUG_ASSERT(null != mElement, "overlay element not found");

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
			if (iter != param.end()) setImageNum(utility::parseInt(iter->second));
		}
	}

	void StaticImage::setImageNum(size_t _num)
	{
		if (_num == mNum) return;
		mNum = _num;

		if (_num == ITEM_NONE) {
			mElementSkin->_setUVSet(FloatRect());
			//mElement->setUV(0, 0, 0, 0);
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
		offset = SkinManager::convertMaterialCoord(offset, mSizeTexture);
		//mElement->setUV(offset.left, offset.top, offset.right, offset.bottom);
		mElementSkin->_setUVSet(offset);
	}

	void StaticImage::setImageInfo(const std::string & _material, const FloatSize & _tile)
	{
		mElementSkin->_setMaterialName(_material);
		mSizeTexture = SkinManager::getMaterialSize(_material);
		mSizeTile = _tile;
		mNum = ITEM_NONE;
	}

	void StaticImage::setImageInfo(const std::string & _material, const FloatRect & _rect, const FloatSize & _tile)
	{
		mElementSkin->_setMaterialName(_material);
		mSizeTexture = SkinManager::getMaterialSize(_material);
		mRectImage = _rect;
		mSizeTile = _tile;
		mNum = ITEM_NONE;
	}

	void StaticImage::setImageMaterial(const std::string & _material)
	{
		mElementSkin->_setMaterialName(_material);
		mSizeTexture = SkinManager::getMaterialSize(_material);
		mNum = ITEM_NONE;

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(_material);
		if (false == material.isNull()) {
			material->getTechnique(0)->setDiffuse(1, 1, 1, 0.5);
			material->getTechnique(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		}

	}

} // namespace MyGUI
