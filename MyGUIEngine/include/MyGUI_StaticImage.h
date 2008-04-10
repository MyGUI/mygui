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
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }


		void setImageInfo(const std::string & _texture, const FloatSize & _tile);
		void setImageInfo(const std::string & _texture, const FloatRect & _rect, const FloatSize & _tile);
		/* Set texture for StaticImage
			_texture file name
		*/
		void setImageTexture(const std::string & _texture);

		inline void setImageRect(const FloatRect & _rect)
		{
			mRectImage = _rect;
			mNum = ITEM_NONE;
		}

		inline void setImageTile(const FloatSize & _tile)
		{
			mSizeTile = _tile;
			mNum = ITEM_NONE;
		}

		void setImageNum(size_t _num);
		inline size_t getImageNum() {return mNum;}

	private:
		// кусок в текстуре наших картинок
		FloatRect mRectImage;
		// размер одной картинки
		FloatSize mSizeTile;
		// размер текстуры
		FloatSize mSizeTexture;
		// текущая картинка
		size_t mNum;

	}; // class StaticImage : public Widget

} // namespace MyGUI

#endif // __MYGUI_STATIC_IMAGE_H__
