/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_StaticImage.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	Ogre::String StaticImage::WidgetTypeName = "StaticImage";
	const size_t IMAGE_MAX_INDEX = 256;

	StaticImage::StaticImage(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mIndexSelect(ITEM_NONE),
		mFrameAdvise(false),
		mCurrentTime(0),
		mCurrentFrame(0)
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
			iter = param.find("ImageCoord");
			if (iter != param.end()) setImageCoord(IntCoord::parse(iter->second));
			iter = param.find("ImageTile");
			if (iter != param.end()) setImageTile(IntSize::parse(iter->second));
			iter = param.find("ImageNum");
			if (iter != param.end()) setImageNum(utility::parseInt(iter->second));
		}
	}

	StaticImage::~StaticImage()
	{
		//Gui::getInstance().removeFrameListener(newDelegate(this, &StaticImage::frameEntered));
	}

	void StaticImage::updateSelectIndex(size_t _index)
	{
		mIndexSelect = _index;

		if (_index == ITEM_NONE) {
			_setUVSet(FloatRect());
			return;
		}

		if (_index >= mItems.size()) {
			_setUVSet(FloatRect());
			return;
		}
		//MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::updateSelectIndex");
		VectorImages::iterator iter = mItems.begin() + _index;

		if (iter->images.size() < 2) {
			if (mFrameAdvise) {
				mFrameAdvise = false;
				Gui::getInstance().eventFrameStart -= newDelegate(this, &StaticImage::frameEntered);
				//Gui::getInstance().removeFrameListener(newDelegate(this, &StaticImage::frameEntered));
			}
		}
		else {
			if ( ! mFrameAdvise) {
				mCurrentTime = 0;
				mCurrentFrame = 0;
				mFrameAdvise = true;
				Gui::getInstance().eventFrameStart += newDelegate(this, &StaticImage::frameEntered);
				//Gui::getInstance().addFrameListener(newDelegate(this, &StaticImage::frameEntered), this);
			}
		}

		if ( ! iter->images.empty()) {
			_setUVSet(iter->images.front());
		}
	}

	void StaticImage::setImageInfo(const std::string & _texture, const IntSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mSizeTile = _tile;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageInfo(const std::string & _texture, const IntRect & _rect, const IntSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mRectImage = _rect;
		mSizeTile = _tile;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageInfo(const std::string & _texture, const IntCoord & _coord, const IntSize & _tile)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);
		mSizeTile = _tile;
		mRectImage.left = _coord.left;
		mRectImage.top = _coord.top;
		mRectImage.right = _coord.left + _coord.width;
		mRectImage.bottom = _coord.top + _coord.height;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageTile(const IntSize & _tile)
	{
		mSizeTile = _tile;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageRect(const IntRect & _rect)
	{
		mRectImage = _rect;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageCoord(const IntCoord & _coord)
	{
		mRectImage.left = _coord.left;
		mRectImage.top = _coord.top;
		mRectImage.right = _coord.left + _coord.width;
		mRectImage.bottom = _coord.top + _coord.height;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageTexture(const std::string & _texture)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);

		if (mItems.empty()) _setUVSet(FloatRect(0, 0, 1, 1));
		else {
			recalcIndexes();
			updateSelectIndex(mIndexSelect);
		}
	}

	void StaticImage::recalcIndexes()
	{
		deleteAllItems();

		// если размер нулевой, то ставим размер текстуры
		if (!(mRectImage.right || mRectImage.bottom)) {
			return;
			//mRectImage.right = mSizeTexture.width ? mSizeTexture.width : 1;
			//mRectImage.bottom = mSizeTexture.height ? mSizeTexture.height : 1;
		}

		if (!(mSizeTile.width || mSizeTile.height)) {
			return;
			//mSizeTile.width = mRectImage.width();
			//mSizeTile.height = mRectImage.height();
		}

		//MYGUI_ASSERT(mRectImage.width() >= mSizeTile.width, "argument failed");
		//MYGUI_ASSERT(mRectImage.height() >= mSizeTile.height, "argument failed");

//		if ((count_h * count_v) > 200) {
//			MYGUI_LOG(Warning, "Tile count very mach, rect : " << mRectImage.print() << " tile : " << mSizeTile.print() << " texture : " << mTexture << " indexes : " << count_h * count_v);
//		}

		//deleteAllItems();

		if ((mRectImage.width() < 0) || (mRectImage.height() < 0)) {
			return;
			//mRectImage.left = 0;
			//mRectImage.top = 0;
			//mRectImage.right = mSizeTile.width;
			//mRectImage.bottom = mSizeTile.height;
		}

		size_t count_h = (size_t)(mRectImage.width() / mSizeTile.width);
		size_t count_v = (size_t)(mRectImage.height() / mSizeTile.height);

		if ((count_h * count_v) > IMAGE_MAX_INDEX) {
			MYGUI_LOG(Warning, "Tile count very mach, rect : " << mRectImage.print() << " tile : " << mSizeTile.print() << " texture : " << mTexture << " indexes : " << (count_h * count_v) << " max : " << IMAGE_MAX_INDEX);
			return;
		}

		int pos_h = mRectImage.left;
		int pos_v = mRectImage.top;

		for (size_t v=0; v<count_v; ++v) {
			for (size_t h=0; h<count_h; ++h) {
				addItem(IntCoord(pos_h, pos_v, mSizeTile.width, mSizeTile.height));
				pos_h += mSizeTile.width;
			}
			pos_v += mSizeTile.height;
			pos_h = mRectImage.left;
		}
	}

	void StaticImage::deleteItem(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::deleteItem");

		mItems.erase(mItems.begin() + _index);

		if (mIndexSelect != ITEM_NONE) {
			if (mItems.empty()) updateSelectIndex(ITEM_NONE);
			else if ((_index < mIndexSelect) || (mIndexSelect == mItems.size())) updateSelectIndex(mIndexSelect--);
		}
	}

	void StaticImage::deleteAllItems()
	{
		updateSelectIndex(ITEM_NONE);
		mItems.clear();
	}

	void StaticImage::insertItem(size_t _index, const IntCoord & _item)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItems.size(), "StaticImage::insertItem");
		if (_index == ITEM_NONE) _index = mItems.size();

		VectorImages::iterator iter = mItems.insert(mItems.begin() + _index, ImageItem());

		iter->images.push_back(SkinManager::convertTextureCoord(
			FloatRect(_item.left, _item.top, _item.width, _item.height), mSizeTexture));

		if ((mIndexSelect != ITEM_NONE) && (_index <= mIndexSelect)) updateSelectIndex(mIndexSelect++);
	}

	void StaticImage::setItem(size_t _index, const IntCoord & _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::setItem");

		VectorImages::iterator iter = mItems.begin() + _index;
		iter->images.clear();
		iter->images.push_back(SkinManager::convertTextureCoord(
			FloatRect(_item.left, _item.top, _item.width, _item.height), mSizeTexture));

		if (_index == mIndexSelect) updateSelectIndex(mIndexSelect);
	}

	void StaticImage::frameEntered(float _frame)
	{
		if (mIndexSelect == ITEM_NONE) return;
		VectorImages::iterator iter = mItems.begin() + mIndexSelect;
		if ((iter->images.size() < 2) || (iter->frame_rate == 0)) return;

		mCurrentTime += _frame;

		while (mCurrentTime >= iter->frame_rate) {
			mCurrentTime -= iter->frame_rate;
			mCurrentFrame ++;
			if (mCurrentFrame >= (iter->images.size())) mCurrentFrame = 0;
		}

		_setUVSet(iter->images[mCurrentFrame]);
	}

	void StaticImage::deleteAllItemsFrame(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::clearItemFrame");
		VectorImages::iterator iter = mItems.begin() + _index;
		iter->images.clear();
	}

	void StaticImage::addItemFrame(size_t _index, const IntCoord & _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::addItemFrame");
		VectorImages::iterator iter = mItems.begin() + _index;
		iter->images.push_back(SkinManager::convertTextureCoord(
			FloatRect(_item.left, _item.top, _item.width, _item.height), mSizeTexture));
	}

	void StaticImage::setItemFrameRate(size_t _index, float _rate)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::setItemFrameRate");
		VectorImages::iterator iter = mItems.begin() + _index;
		iter->frame_rate = _rate;
	}

	float StaticImage::getItemFrameRate(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::getItemFrameRate");
		VectorImages::iterator iter = mItems.begin() + _index;
		return iter->frame_rate;
	}

	void StaticImage::addItemFrameDublicate(size_t _index, size_t _indexSourceFrame)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::addItemFrameDublicate");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE(_indexSourceFrame, iter->images.size(), "StaticImage::addItemFrameDublicate");
		iter->images.push_back(iter->images[_indexSourceFrame]);
	}

	void StaticImage::insertItemFrame(size_t _index, size_t _indexFrame, const IntCoord & _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::insertItemFrame");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE_INSERT(_indexFrame, iter->images.size(), "StaticImage::insertItemFrame");
		if (_indexFrame == ITEM_NONE) _indexFrame = iter->images.size() - 1;

		iter->images.insert(iter->images.begin() + _indexFrame, 
			SkinManager::convertTextureCoord(FloatRect(_item.left, _item.top, _item.width, _item.height), mSizeTexture));
	}

	void StaticImage::insertItemFrameDublicate(size_t _index, size_t _indexFrame, size_t _indexSourceFrame)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::insertItemFrameDublicate");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE_INSERT(_indexFrame, iter->images.size(), "StaticImage::insertItemFrameDublicate");
		if (_indexFrame == ITEM_NONE) _indexFrame = iter->images.size() - 1;

		MYGUI_ASSERT_RANGE(_indexSourceFrame, iter->images.size(), "StaticImage::insertItemFrameDublicate");

		iter->images.insert(iter->images.begin() + _indexFrame, iter->images[_indexSourceFrame]);		
	}

	void StaticImage::setItemFrame(size_t _index, size_t _indexFrame, const IntCoord & _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::setItemFrame");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE(_indexFrame, iter->images.size(), "StaticImage::setItemFrame");

		iter->images[_indexFrame] = SkinManager::convertTextureCoord(
			FloatRect(_item.left, _item.top, _item.width, _item.height), mSizeTexture);
	}

	void StaticImage::deleteItemFrame(size_t _index, size_t _indexFrame)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "StaticImage::deleteItemFrame");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE_INSERT(_indexFrame, iter->images.size(), "StaticImage::deleteItemFrame");
		if (_indexFrame == ITEM_NONE) _indexFrame = iter->images.size() - 1;

		iter->images.erase(iter->images.begin() + _indexFrame);
	}

	void StaticImage::setItemSelect(const std::string & _name)
	{
		size_t index = ITEM_NONE;
		MapName::iterator iter = mMapIndexName.find(_name);
		if (iter != mMapIndexName.end()) index = iter->second;
		setItemSelect(index);
	}

	void StaticImage::addItemNames(const std::string & _name)
	{
		typedef std::vector<std::string> VectorString;
		VectorString names = utility::split(_name);
		size_t index = 0;
		for (VectorString::iterator iter=names.begin(); iter!=names.end(); ++iter) {
			mMapIndexName[*iter] = index;
			index++;
		}
	}

	void StaticImage::addItemInfo(const std::string & _info)
	{
		typedef std::vector<std::string> VectorString;

		VectorString split = utility::split(_info, "|");
		if (split.size() < 3) {
			MYGUI_LOG(Warning, "StaticImage::addItemInfo - error parse '" << _info << "'");
			return;
		}

		VectorString::iterator iter = split.begin();
		float rate = utility::parseFloat(*iter);
		++iter;
		IntSize size = IntSize::parse(*iter);
		++iter;
		IntPoint point = IntPoint::parse(*iter);
		++iter;

		addItem(IntCoord(point, size));
		size_t index = mItems.size() - 1;
		setItemFrameRate(index, rate);

		for (;iter!=split.end(); ++iter) {
			addItemFrame(index, MyGUI::IntCoord(IntPoint::parse(*iter), size));
		}
	}

} // namespace MyGUI
