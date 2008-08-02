/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_STATIC_IMAGE_H__
#define __MYGUI_STATIC_IMAGE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
//#include "MyGUI_FrameListener.h"

namespace MyGUI
{

	struct ImageItem
	{
		ImageItem() : frame_rate(0) {}

		float frame_rate;
		std::vector<FloatRect> images;
	};

	typedef std::vector<ImageItem> VectorImages;

	class _MyGUIExport StaticImage : public Widget//, public FrameListener
	{
		// для вызова закрытого конструктора
		friend class factory::StaticImageFactory;

	protected:
		StaticImage(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		~StaticImage();
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/* Set texture and size of image _tile
			@param _texture file name
			@param _tile size
		*/
		void setImageInfo(const std::string & _texture, const IntSize & _tile);
		/* Set texture and size of image _tile
			@param _texture file name
			@param _rect - part of texture where we take tiles
			@param _tile size
		*/
		void setImageInfo(const std::string & _texture, const IntRect & _rect, const IntSize & _tile);
		/* Set texture and size of image _tile
			@param _texture file name
			@param _coord - part of texture where we take tiles
			@param _tile size
		*/
		void setImageInfo(const std::string & _texture, const IntCoord & _coord, const IntSize & _tile);
		/* Set texture
			@param _texture file name
		*/
		void setImageTexture(const std::string & _texture);

		/** Set _rect - part of texture where we take tiles */
		void setImageRect(const IntRect & _rect);

		/** Set _coord - part of texture where we take tiles */
		void setImageCoord(const IntCoord & _coord);

		/** Set _tile size */
		void setImageTile(const IntSize & _tile);

		/** Set current tile index
			@param _index - tile index
			@remarks Tiles in file start numbering from left to right and from top to bottom.
			\n \bExample:\n
			<pre>
				+---+---+---+
				| 1 | 2 | 3 |
				+---+---+---+
				| 4 | 5 | 6 |
				+---+---+---+
			</pre>
		*/
		inline void setImageNum(size_t _index) { setItemSelect(_index); }
		/** Get current tile index */
		inline size_t getImageNum() {return mIndexSelect;}



		//! Get number of items
		inline size_t getItemCount() { return mItems.size(); }

		//! Select specified _index
		inline void setItemSelect(size_t _index) { if (mIndexSelect != _index) updateSelectIndex(_index); }
		//! Select specified name
		void setItemSelect(const std::string & _name);
		//! Get index of selected item (ITEM_NONE if none selected)
		inline size_t getItemSelect() { return mIndexSelect; }
		//! Reset item selection
		inline void resetItemSelect() { setItemSelect(ITEM_NONE); }

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const IntCoord & _item);
		//! Add an item to the end of a list
		inline void addItem(const IntCoord & _item) { insertItem(ITEM_NONE, _item); }
		//! Replace an item at a specified position
		void setItem(size_t _index, const IntCoord & _item);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		// работа с фреймами анимированных индексов
		void addItemFrame(size_t _index, const IntCoord & _item);
		void insertItemFrame(size_t _index, size_t _indexFrame, const IntCoord & _item);

		void addItemFrameDublicate(size_t _index, size_t _indexSourceFrame);
		void insertItemFrameDublicate(size_t _index, size_t _indexFrame, size_t _indexSourceFrame);

		void setItemFrame(size_t _index, size_t _indexFrame, const IntCoord & _item);

		void deleteItemFrame(size_t _index, size_t _indexFrame);
		void deleteAllItemsFrame(size_t _index);

		void setItemFrameRate(size_t _index, float _rate);
		float getItemFrameRate(size_t _index);

		inline void addItemName(const std::string & _name, size_t _index) { mMapIndexName[_name] = _index; }
		void addItemNames(const std::string & _name);

	private:
		void frameEntered(float _frame);

		void recalcIndexes();
		void updateSelectIndex(size_t _index);

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

		typedef std::map<std::string, size_t> MapName;
		MapName mMapIndexName;

	}; // class StaticImage : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_H__
