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

namespace MyGUI
{

	class _MyGUIExport StaticImage : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::StaticImageFactory;

	protected:
		StaticImage(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
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
		/* Set texture
			@param _texture file name
		*/
		void setImageTexture(const std::string & _texture);

		/** Set _rect - part of texture where we take tiles */
		void setImageRect(const IntRect & _rect);

		/** Set _tile size */
		void setImageTile(const IntSize & _tile);

		/** Set current tile number
			@param _num - tile number
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
		void setImageNum(size_t _num);
		/** Get current tile number */
		inline size_t getImageNum() {return mNum;}

	private:
		// кусок в текстуре наших картинок
		IntRect mRectImage;
		// размер одной картинки
		IntSize mSizeTile;
		// размер текстуры
		IntSize mSizeTexture;
		// текущая картинка
		size_t mNum;

	}; // class StaticImage : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_H__
