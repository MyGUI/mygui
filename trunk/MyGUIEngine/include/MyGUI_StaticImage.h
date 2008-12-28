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
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_ImageInfo.h"
#include "MyGUI_Guid.h"

namespace MyGUI
{

	class MYGUI_EXPORT StaticImage : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<StaticImage>;

		MYGUI_RTTI_CHILD_HEADER( StaticImage, Widget );

	public:

		//------------------------------------------------------------------------------------------------//
		// The simple interface
		//------------------------------------------------------------------------------------------------//

		/* Set texture and size of image _tile
			@param _texture file name or texture name in Ogre
			@param _coord - part of texture where we take tiles
			@param _tile size
		*/
		void setImageInfo(const std::string & _texture, const IntCoord & _coord, const IntSize & _tile);

		/* Set texture
			@param _texture file name or texture name in Ogre
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
				| 0 | 1 | 2 |
				+---+---+---+
				| 3 | 4 | 5 |
				+---+---+---+
			</pre>
		*/
		void setImageIndex(size_t _index) { setItemSelect(_index); }
		/** Get current tile index */
		size_t getImageIndex() { return getItemSelect(); }

		//------------------------------------------------------------------------------------------------//
		// The expanded interface
		//------------------------------------------------------------------------------------------------//

		//! Get number of items
		size_t getItemCount() { return mItems.size(); }

		//! Select specified _index
		void setItemSelect(size_t _index) { if (mIndexSelect != _index) updateSelectIndex(_index); }
		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemSelect() { return mIndexSelect; }
		//! Reset item selection
		void resetItemSelect() { setItemSelect(ITEM_NONE); }

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const IntCoord & _item);
		//! Add an item to the end of a list
		void addItem(const IntCoord & _item) { insertItem(ITEM_NONE, _item); }
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

		//------------------------------------------------------------------------------------------------//
		// The interface with support of resources
		//------------------------------------------------------------------------------------------------//

		void setItemResourceInfo(const ImageIndexInfo & _info);

		ResourceImageSetPtr getItemResource() { return mResource; }

		bool setItemResource(const Guid & _id);
		bool setItemResource(const std::string & _name);
		void setItemResource(ResourceImageSetPtr _resource);
		void setItemResource(ResourceImageSetPtr _resource, const std::string & _group, const std::string & _name);

		void setItemGroup(const std::string & _group);
		void setItemName(const std::string & _name);

		
	protected:
		StaticImage(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~StaticImage();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

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

		ResourceImageSetPtr mResource;
		std::string mItemName;
		std::string mItemGroup;

	}; // class StaticImage : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_H__
