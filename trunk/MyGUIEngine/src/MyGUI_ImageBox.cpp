/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	const size_t IMAGE_MAX_INDEX = 256;

	ImageBox::ImageBox() :
		mIndexSelect(ITEM_NONE),
		mFrameAdvise(false),
		mCurrentTime(0),
		mCurrentFrame(0),
		mResource(nullptr)
	{
	}

	void ImageBox::shutdownOverride()
	{
		frameAdvise(false);

		Base::shutdownOverride();
	}

	void ImageBox::setImageInfo(const std::string& _texture, const IntCoord& _coord, const IntSize& _tile)
	{
		mCurrentTextureName = _texture;
		mSizeTexture = texture_utility::getTextureSize(mCurrentTextureName);

		mSizeTile = _tile;
		mRectImage.left = _coord.left;
		mRectImage.top = _coord.top;
		mRectImage.right = _coord.left + _coord.width;
		mRectImage.bottom = _coord.top + _coord.height;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void ImageBox::setImageTile(const IntSize& _tile)
	{
		mSizeTile = _tile;

		// если размер еще не установлен, то ставим тот что у тайла
		if (mRectImage.empty()) mRectImage.set(0, 0, _tile.width, _tile.height);
		//если индекса еще нет, то ставим 0
		if (mIndexSelect == ITEM_NONE) mIndexSelect = 0;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void ImageBox::setImageCoord(const IntCoord& _coord)
	{
		mRectImage.left = _coord.left;
		mRectImage.top = _coord.top;
		mRectImage.right = _coord.left + _coord.width;
		mRectImage.bottom = _coord.top + _coord.height;

		setImageRect(mRectImage);
	}

	void ImageBox::setImageRect(const IntRect& _rect)
	{
		mRectImage = _rect;

		// если тайл еще не установлен, то ставим тот что у координат
		if (mSizeTile.empty()) mSizeTile.set(_rect.width(), _rect.height());
		//если индекса еще нет, то ставим 0
		if (mIndexSelect == ITEM_NONE) mIndexSelect = 0;

		recalcIndexes();
		updateSelectIndex(mIndexSelect);
	}

	void ImageBox::setImageTexture(const std::string& _texture)
	{
		mCurrentTextureName = _texture;
		mSizeTexture = texture_utility::getTextureSize(mCurrentTextureName);

		// если первый раз, то ставим во всю текстуру
		if (mItems.empty())
		{
			_setUVSet(FloatRect(0, 0, 1, 1));
			_setTextureName(mCurrentTextureName);
		}
		else
		{
			recalcIndexes();
			updateSelectIndex(mIndexSelect);
		}
	}

	void ImageBox::recalcIndexes()
	{
		mItems.clear();

		if ((mRectImage.right <= mRectImage.left) || (mRectImage.bottom <= mRectImage.top))
			return;
		if ((mSizeTile.width <= 0) || (mSizeTile.height <= 0))
			return;

		size_t count_h = (size_t)(mRectImage.width() / mSizeTile.width);
		size_t count_v = (size_t)(mRectImage.height() / mSizeTile.height);

		if ((count_h * count_v) > IMAGE_MAX_INDEX)
		{
			MYGUI_LOG(Warning, "Tile count very mach, rect : " << mRectImage.print() << " tile : " << mSizeTile.print() << " texture : " << _getTextureName() << " indexes : " << (count_h * count_v) << " max : " << IMAGE_MAX_INDEX);
			return;
		}

		int pos_h = mRectImage.left;
		int pos_v = mRectImage.top;

		for (size_t v = 0; v < count_v; ++v)
		{
			for (size_t h = 0; h < count_h; ++h)
			{
				addItem(IntCoord(pos_h, pos_v, mSizeTile.width, mSizeTile.height));
				pos_h += mSizeTile.width;
			}
			pos_v += mSizeTile.height;
			pos_h = mRectImage.left;
		}
	}

	void ImageBox::updateSelectIndex(size_t _index)
	{
		mIndexSelect = _index;

		if ((_index == ITEM_NONE) || (_index >= mItems.size()))
		{
			_setTextureName("");
			return;
		}
		else
		{
			_setTextureName(mCurrentTextureName);
		}

		VectorImages::iterator iter = mItems.begin() + _index;

		if (iter->images.size() < 2)
		{
			frameAdvise(false);
		}
		else
		{
			if ( ! mFrameAdvise)
			{
				mCurrentTime = 0;
				mCurrentFrame = 0;
			}
			frameAdvise(true);
		}

		if ( ! iter->images.empty())
		{
			_setUVSet(iter->images.front());
		}
	}

	void ImageBox::deleteItem(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::deleteItem");

		mItems.erase(mItems.begin() + _index);

		if (mIndexSelect != ITEM_NONE)
		{
			if (mItems.empty()) updateSelectIndex(ITEM_NONE);
			else if ((_index < mIndexSelect) || (mIndexSelect == mItems.size())) updateSelectIndex(mIndexSelect--);
		}
	}

	void ImageBox::deleteAllItems()
	{
		updateSelectIndex(ITEM_NONE);
		mItems.clear();
	}

	void ImageBox::insertItem(size_t _index, const IntCoord& _item)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItems.size(), "ImageBox::insertItem");
		if (_index == ITEM_NONE) _index = mItems.size();

		VectorImages::iterator iter = mItems.insert(mItems.begin() + _index, ImageItem());

		iter->images.push_back(CoordConverter::convertTextureCoord(_item, mSizeTexture));

		if ((mIndexSelect != ITEM_NONE) && (_index <= mIndexSelect)) updateSelectIndex(mIndexSelect++);
	}

	void ImageBox::setItem(size_t _index, const IntCoord& _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::setItem");

		VectorImages::iterator iter = mItems.begin() + _index;
		iter->images.clear();
		iter->images.push_back(CoordConverter::convertTextureCoord(_item, mSizeTexture));

		if (_index == mIndexSelect) updateSelectIndex(mIndexSelect);
	}

	void ImageBox::frameEntered(float _frame)
	{
		if (mIndexSelect == ITEM_NONE) return;

		if (mItems.empty()) return;
		VectorImages::iterator iter = mItems.begin() + mIndexSelect;
		if ((iter->images.size() < 2) || (iter->frame_rate == 0)) return;

		mCurrentTime += _frame;

		while (mCurrentTime >= iter->frame_rate)
		{
			mCurrentTime -= iter->frame_rate;
			mCurrentFrame ++;
			if (mCurrentFrame >= (iter->images.size())) mCurrentFrame = 0;
		}

		_setUVSet(iter->images[mCurrentFrame]);
	}

	void ImageBox::deleteAllItemFrames(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::clearItemFrame");
		VectorImages::iterator iter = mItems.begin() + _index;
		iter->images.clear();
	}

	void ImageBox::addItemFrame(size_t _index, const IntCoord& _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::addItemFrame");
		VectorImages::iterator iter = mItems.begin() + _index;
		iter->images.push_back(CoordConverter::convertTextureCoord(_item, mSizeTexture));
	}

	void ImageBox::setItemFrameRate(size_t _index, float _rate)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::setItemFrameRate");
		VectorImages::iterator iter = mItems.begin() + _index;
		iter->frame_rate = _rate;
	}

	float ImageBox::getItemFrameRate(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::getItemFrameRate");
		VectorImages::iterator iter = mItems.begin() + _index;
		return iter->frame_rate;
	}

	void ImageBox::addItemFrameDublicate(size_t _index, size_t _indexSourceFrame)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::addItemFrameDublicate");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE(_indexSourceFrame, iter->images.size(), "ImageBox::addItemFrameDublicate");
		iter->images.push_back(iter->images[_indexSourceFrame]);
	}

	void ImageBox::insertItemFrame(size_t _index, size_t _indexFrame, const IntCoord& _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::insertItemFrame");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE_INSERT(_indexFrame, iter->images.size(), "ImageBox::insertItemFrame");
		if (_indexFrame == ITEM_NONE) _indexFrame = iter->images.size() - 1;

		iter->images.insert(iter->images.begin() + _indexFrame,
			CoordConverter::convertTextureCoord(_item, mSizeTexture));
	}

	void ImageBox::insertItemFrameDublicate(size_t _index, size_t _indexFrame, size_t _indexSourceFrame)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::insertItemFrameDublicate");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE_INSERT(_indexFrame, iter->images.size(), "ImageBox::insertItemFrameDublicate");
		if (_indexFrame == ITEM_NONE) _indexFrame = iter->images.size() - 1;

		MYGUI_ASSERT_RANGE(_indexSourceFrame, iter->images.size(), "ImageBox::insertItemFrameDublicate");

		iter->images.insert(iter->images.begin() + _indexFrame, iter->images[_indexSourceFrame]);
	}

	void ImageBox::setItemFrame(size_t _index, size_t _indexFrame, const IntCoord& _item)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::setItemFrame");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE(_indexFrame, iter->images.size(), "ImageBox::setItemFrame");

		iter->images[_indexFrame] = CoordConverter::convertTextureCoord(_item, mSizeTexture);
	}

	void ImageBox::deleteItemFrame(size_t _index, size_t _indexFrame)
	{
		MYGUI_ASSERT_RANGE(_index, mItems.size(), "ImageBox::deleteItemFrame");

		VectorImages::iterator iter = mItems.begin() + _index;
		MYGUI_ASSERT_RANGE_INSERT(_indexFrame, iter->images.size(), "ImageBox::deleteItemFrame");
		if (_indexFrame == ITEM_NONE) _indexFrame = iter->images.size() - 1;

		iter->images.erase(iter->images.begin() + _indexFrame);
	}

	void ImageBox::setItemResourceInfo(const ImageIndexInfo& _info)
	{
		mCurrentTextureName = _info.texture;
		mSizeTexture = texture_utility::getTextureSize(mCurrentTextureName);

		mItems.clear();

		if (_info.frames.size() != 0)
		{
			std::vector<IntPoint>::const_iterator iter = _info.frames.begin();

			addItem(IntCoord(*iter, _info.size));
			setItemFrameRate(0, _info.rate);

			for (++iter; iter != _info.frames.end(); ++iter)
			{
				addItemFrame(0, MyGUI::IntCoord(*iter, _info.size));
			}

		}

		mIndexSelect = 0;
		updateSelectIndex(mIndexSelect);
	}

	bool ImageBox::setItemResource(const std::string& _name)
	{
		IResourcePtr resource = ResourceManager::getInstance().getByName(_name, false);
		setItemResourcePtr(resource ? resource->castType<ResourceImageSet>() : nullptr);
		return resource != nullptr;
	}

	void ImageBox::setItemResourcePtr(ResourceImageSetPtr _resource)
	{
		if (mResource == _resource)
			return;

		// если первый раз то устанавливаем дефолтное
		if (mResource == nullptr && _resource != nullptr)
		{
			if (mItemGroup.empty())
			{
				EnumeratorGroupImage iter_group = _resource->getEnumerator();
				while (iter_group.next())
				{
					mItemGroup = iter_group.current().name;
					if (mItemName.empty() && !iter_group.current().indexes.empty())
					{
						mItemName = iter_group.current().indexes[0].name;
					}
					break;
				}
			}
			else if (mItemName.empty())
			{
				EnumeratorGroupImage iter_group = _resource->getEnumerator();
				while (iter_group.next())
				{
					if (mItemGroup == iter_group.current().name)
					{
						if (!iter_group.current().indexes.empty())
						{
							mItemName = iter_group.current().indexes[0].name;
							break;
						}
					}
				}
			}
		}

		mResource = _resource;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void ImageBox::setItemGroup(const std::string& _group)
	{
		if (mItemGroup == _group)
			return;

		mItemGroup = _group;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void ImageBox::setItemName(const std::string& _name)
	{
		if (mItemName == _name)
			return;

		mItemName = _name;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void ImageBox::setItemResourceInfo(ResourceImageSetPtr _resource, const std::string& _group, const std::string& _name)
	{
		mResource = _resource;
		mItemGroup = _group;
		mItemName = _name;
		if (!mResource || mItemGroup.empty() || mItemName.empty()) updateSelectIndex(ITEM_NONE);
		else setItemResourceInfo(mResource->getIndexInfo(mItemGroup, mItemName));
	}

	void ImageBox::frameAdvise(bool _advise)
	{
		if ( _advise )
		{
			if ( ! mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate( this, &ImageBox::frameEntered );
				mFrameAdvise = true;
			}
		}
		else
		{
			if ( mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate( this, &ImageBox::frameEntered );
				mFrameAdvise = false;
			}
		}
	}

	void ImageBox::setImageIndex(size_t _index)
	{
		setItemSelect(_index);
	}

	size_t ImageBox::getImageIndex() const
	{
		return getItemSelect();
	}

	IntSize ImageBox::getImageSize() const
	{
		return mSizeTexture;
	}

	void ImageBox::setItemSelect(size_t _index)
	{
		if (mIndexSelect != _index) updateSelectIndex(_index);
	}

	void ImageBox::_setUVSet(const FloatRect& _rect)
	{
		if (nullptr != getSubWidgetMain())
			getSubWidgetMain()->_setUVSet(_rect);
	}

	void ImageBox::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{ImageBox, ImageTexture, string} Текстура для виджета.
		if (_key == "ImageTexture")
			setImageTexture(_value);

		/// @wproperty{ImageBox, ImageCoord, int int int int} Координаты в текстуре.
		else if (_key == "ImageCoord")
			setImageCoord(utility::parseValue<IntCoord>(_value));

		/// @wproperty{ImageBox, ImageTile, int int} Размер тайла текстуры.
		else if (_key == "ImageTile")
			setImageTile(utility::parseValue<IntSize>(_value));

		/// @wproperty{ImageBox, ImageIndex, size_t} Индекс тайла в текстуре.
		else if (_key == "ImageIndex")
			setItemSelect(utility::parseValue<size_t>(_value));

		/// @wproperty{ImageBox, ImageResource, string} Имя ресурса картинки.
		else if (_key == "ImageResource")
			setItemResource(_value);

		/// @wproperty{ImageBox, ImageGroup, string} Имя группы картинки в ресурсе.
		else if (_key == "ImageGroup")
			setItemGroup(_value);

		/// @wproperty{ImageBox, ImageName, string} Имя картинки в группе ресурса.
		else if (_key == "ImageName")
			setItemName(_value);

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	size_t ImageBox::getItemCount() const
	{
		return mItems.size();
	}

	size_t ImageBox::getItemSelect() const
	{
		return mIndexSelect;
	}

	void ImageBox::resetItemSelect()
	{
		setItemSelect(ITEM_NONE);
	}

	void ImageBox::addItem(const IntCoord& _item)
	{
		insertItem(ITEM_NONE, _item);
	}

	ResourceImageSetPtr ImageBox::getItemResource() const
	{
		return mResource;
	}

} // namespace MyGUI
