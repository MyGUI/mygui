/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_StaticImage.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( StaticImage, Widget );

	const size_t IMAGE_MAX_INDEX = 256;

	StaticImage::StaticImage(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mIndexSelect(ITEM_NONE),
		mFrameAdvise(false),
		mCurrentTime(0),
		mCurrentFrame(0),
		mResource(null)
	{
		// первоначальная инициализация
		MYGUI_DEBUG_ASSERT(null != mMainSkin, "need one subskin");

		// парсим свойства
		const MapString & properties = _info->getProperties();
		if ( ! properties.empty() ) {
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("ImageTexture")) != properties.end()) setImageTexture(iter->second);
			if ((iter = properties.find("ImageRect")) != properties.end()) setImageRect(IntRect::parse(iter->second));
			if ((iter = properties.find("ImageCoord")) != properties.end()) setImageCoord(IntCoord::parse(iter->second));
			if ((iter = properties.find("ImageTile")) != properties.end()) setImageTile(IntSize::parse(iter->second));
			if ((iter = properties.find("ImageIndex")) != properties.end()) setImageIndex(utility::parseInt(iter->second));
			if ((iter = properties.find("ImageResource")) != properties.end()) setItemResource(iter->second);
			if ((iter = properties.find("ImageGroup")) != properties.end()) setItemGroup(iter->second);
			if ((iter = properties.find("ImageName")) != properties.end()) setItemName(iter->second);
		}
	}

	StaticImage::~StaticImage()
	{
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

		// если размер еще не установлен, то ставим тот что у тайла
		if (mRectImage.empty()) mRectImage.set(0, 0, _tile.width, _tile.height);
		//если индекса еще нет, то ставим 0
		if (mIndexSelect == ITEM_NONE) mIndexSelect = 0;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageCoord(const IntCoord & _coord)
	{
		mRectImage.left = _coord.left;
		mRectImage.top = _coord.top;
		mRectImage.right = _coord.left + _coord.width;
		mRectImage.bottom = _coord.top + _coord.height;

		// если тайл еще не установлен, то ставим тот что у координат
		if (mSizeTile.empty()) mSizeTile = _coord.size();
		//если индекса еще нет, то ставим 0
		if (mIndexSelect == ITEM_NONE) mIndexSelect = 0;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageRect(const IntRect & _rect)
	{
		mRectImage= _rect;

		// если тайл еще не установлен, то ставим тот что у координат
		if (mSizeTile.empty()) mSizeTile.set(_rect.width(), _rect.height());
		//если индекса еще нет, то ставим 0
		if (mIndexSelect == ITEM_NONE) mIndexSelect = 0;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void StaticImage::setImageTexture(const std::string & _texture)
	{
		_setTextureName(_texture);
		mSizeTexture = SkinManager::getTextureSize(_texture);

		// если первый раз, то ставим во всю текстуру
		if (mItems.empty()) _setUVSet(FloatRect(0, 0, 1, 1));
		else {
			recalcIndexes();
			updateSelectIndex(mIndexSelect);
		}
	}

	void StaticImage::recalcIndexes()
	{
		mItems.clear();

		if ((mRectImage.right <= mRectImage.left) || (mRectImage.bottom <= mRectImage.top)) return;
		if ((mSizeTile.width <= 0) || (mSizeTile.height <= 0)) return;

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

	void StaticImage::updateSelectIndex(size_t _index)
	{
		mIndexSelect = _index;

		if ((_index == ITEM_NONE) || (_index >= mItems.size())) {
			_setUVSet(FloatRect());
			return;
		}

		VectorImages::iterator iter = mItems.begin() + _index;

		if (iter->images.size() < 2) {
			if (mFrameAdvise) {
				mFrameAdvise = false;
				Gui::getInstance().eventFrameStart -= newDelegate(this, &StaticImage::frameEntered);
			}
		}
		else {
			if ( ! mFrameAdvise) {
				mCurrentTime = 0;
				mCurrentFrame = 0;
				mFrameAdvise = true;
				Gui::getInstance().eventFrameStart += newDelegate(this, &StaticImage::frameEntered);
			}
		}

		if ( ! iter->images.empty()) {
			_setUVSet(iter->images.front());
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

	void StaticImage::setItemResourceInfo(const ImageIndexInfo & _info)
	{
		_setTextureName(_info.texture);
		mSizeTexture = SkinManager::getTextureSize(_info.texture);

		mItems.clear();

		if (_info.frames.size() != 0) {
			std::vector<IntPoint>::const_iterator iter = _info.frames.begin();

			addItem(IntCoord(*iter, _info.size));
			setItemFrameRate(0, _info.rate);

			for (++iter; iter!=_info.frames.end(); ++iter) {
				addItemFrame(0, MyGUI::IntCoord(*iter, _info.size));
			}

		}

		mIndexSelect = 0;
		updateSelectIndex(mIndexSelect);
	}

	bool StaticImage::setItemResource(const Guid & _id)
	{
		IResourcePtr resource = ResourceManager::getInstance().getResource(_id, false);
		setItemResource(resource ? resource->castType<ResourceImageSet>() : null);
		return resource != null;
	}

	bool StaticImage::setItemResource(const std::string & _name)
	{
		IResourcePtr resource = ResourceManager::getInstance().getResource(_name, false);
		setItemResource(resource ? resource->castType<ResourceImageSet>() : null);
		return resource != null;
	}

	void StaticImage::setItemResource(ResourceImageSetPtr _resource)
	{
		mResource = _resource;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void StaticImage::setItemGroup(const std::string & _group)
	{
		mItemGroup = _group;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void StaticImage::setItemName(const std::string & _name)
	{
		mItemName = _name;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void StaticImage::setItemResource(ResourceImageSetPtr _resource, const std::string & _group, const std::string & _name)
	{
		mResource = _resource;
		mItemGroup = _group;
		mItemName = _name;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

} // namespace MyGUI
