/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_IMAGE_BOX_H_
#define MYGUI_IMAGE_BOX_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_ImageInfo.h"

namespace MyGUI
{

	/** \brief @wpage{ImageBox}
		ImageBox widget description should be here.
	*/
	class MYGUI_EXPORT ImageBox :
		public Widget,
		public MemberObsolete<ImageBox>
	{
		MYGUI_RTTI_DERIVED( ImageBox )

	public:
		ImageBox();

		//------------------------------------------------------------------------------//
		// The simple interface
		//------------------------------------------------------------------------------//

		/* Set texture and size of image _tile
			@param _texture file name or texture name
			@param _coord - part of texture where we take tiles
			@param _tile size
		*/
		void setImageInfo(const std::string& _texture, const IntCoord& _coord, const IntSize& _tile);

		/* Set texture
			@param _texture file name or texture name
		*/
		void setImageTexture(const std::string& _value);

		/** Set _rect - part of texture where we take tiles */
		void setImageRect(const IntRect& _value);

		/** Set _coord - part of texture where we take tiles */
		void setImageCoord(const IntCoord& _value);

		/** Set _tile size */
		void setImageTile(const IntSize& _value);

		/** Set current tile index
			@param _index - tile index
			@remarks Tiles in file start numbering from left to right and from top to bottom.
			\n For example:\n
			<pre>
				+---+---+---+
				| 0 | 1 | 2 |
				+---+---+---+
				| 3 | 4 | 5 |
				+---+---+---+
			</pre>
		*/
		void setImageIndex(size_t _index);
		/** Get current tile index */
		size_t getImageIndex() const;

		/** Get size of currently used image */
		IntSize getImageSize() const;

		//------------------------------------------------------------------------------//
		// The expanded interface
		//------------------------------------------------------------------------------//

		//! Get number of items
		size_t getItemCount() const;

		//! Select specified _index
		void setItemSelect(size_t _index);
		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemSelect() const;
		//! Reset item selection
		void resetItemSelect();

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const IntCoord& _item);
		//! Add an item to the end of a list
		void addItem(const IntCoord& _item);
		//! Replace an item at a specified position
		void setItem(size_t _index, const IntCoord& _item);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		// работа с фреймами анимированных индексов
		/** Add frame
			@param _index Image item index
			@param _item Frame coordinates at image texture
		*/
		void addItemFrame(size_t _index, const IntCoord& _item);

		/** Insert frame
			@param _index Image item index
			@param _indexFrame Frame index where we insert frame
			@param _item Frame coordinates at image texture
		*/
		void insertItemFrame(size_t _index, size_t _indexFrame, const IntCoord& _item);

		/** Add copy of frame (similar to ImageBox::addItemFrame but we copy frame coordinates)
			@param _index Image item index
			@param _indexSourceFrame Frame index of frame that we copying
		*/
		void addItemFrameDuplicate(size_t _index, size_t _indexSourceFrame);

		/** Insert copy of frame (similar to ImageBox::insertItemFrame but we copy frame coordinates)
			@param _index Image item index
			@param _indexFrame Frame index where we insert frame
			@param _indexSourceFrame Frame index of frame that we copying
		*/
		void insertItemFrameDuplicate(size_t _index, size_t _indexFrame, size_t _indexSourceFrame);

		/** Change frame
			@param _index Image item index
			@param _indexFrame Frame index to change
			@param _item Frame coordinates at image texture
		*/
		void setItemFrame(size_t _index, size_t _indexFrame, const IntCoord& _item);

		/** Delete frame
			@param _index Image item index
			@param _indexFrame Frame index that we delete
		*/
		void deleteItemFrame(size_t _index, size_t _indexFrame);

		/** Delete all frames
			@param _index Image item index
		*/
		void deleteAllItemFrames(size_t _index);

		/** Set item frame rate
			@param _index Image item index
			@param _rate Duration of one frame in seconds
		*/
		void setItemFrameRate(size_t _index, float _rate);

		/** Get item frame rate
			@param _index Image item index
		*/
		float getItemFrameRate(size_t _index);

		//------------------------------------------------------------------------------//
		// The interface with support of resources
		//------------------------------------------------------------------------------//

		/** Select current items resource used in ImageBox
			@param _name Resource name
			@return false if resource with such name not exist
		*/
		bool setItemResource(const std::string& _name);

		/** Select current item group */
		void setItemGroup(const std::string& _value);
		/** Select current item mane */
		void setItemName(const std::string& _value);

		/** Select current items resource used in ImageBox
			@param _resource Resource pointer
		*/
		void setItemResourcePtr(ResourceImageSetPtr _resource);
		/** Set current item */
		void setItemResourceInfo(const ImageIndexInfo& _info);

		/** Get current items resource used in ImageBox */
		ResourceImageSetPtr getItemResource() const;
		/** Select current item resource, group and name */
		void setItemResourceInfo(ResourceImageSetPtr _resource, const std::string& _group, const std::string& _name);

	protected:
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void frameEntered(float _frame);

		void recalcIndexes();
		void updateSelectIndex(size_t _index);

		void frameAdvise(bool _advise);

		void _setUVSet(const FloatRect& _rect);

	private:
		// кусок в текстуре наших картинок
		IntRect mRectImage;
		// размер одной картинки
		IntSize mSizeTile;
		// размер текстуры
		IntSize mSizeTexture;
		// текущая картинка
		size_t mIndexSelect;

		VectorImages mItems;

		bool mFrameAdvise;
		float mCurrentTime;
		size_t mCurrentFrame;

		ResourceImageSetPtr mResource;
		std::string mItemName;
		std::string mItemGroup;
		std::string mCurrentTextureName;
	};

} // namespace MyGUI

#endif // MYGUI_IMAGE_BOX_H_
