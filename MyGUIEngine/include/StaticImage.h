#ifndef _STATICIMAGE_H_
#define _STATICIMAGE_H_

#include"Prerequest.h"
#include "Widget.h"

namespace MyGUI
{

	class _MyGUIExport StaticImage : public Widget
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<StaticImage>;

	protected:
		StaticImage(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("StaticImage"); return type;};

		inline void setImageInfo(const std::string & _material, const FloatSize & _tile)
		{
			m_element->setMaterialName(_material);
			m_sizeTexture = SkinManager::getMaterialSize(_material);
			m_sizeTile = _tile;
			m_num = (size_t)-1;
		}

		inline void setImageInfo(const std::string & _material, const FloatRect & _rect, const FloatSize & _tile)
		{
			m_element->setMaterialName(_material);
			m_sizeTexture = SkinManager::getMaterialSize(_material);
			m_rectImage = _rect;
			m_sizeTile = _tile;
			m_num = (size_t)-1;
		}

		inline void setImageMaterial(const std::string & _material)
		{
			m_element->setMaterialName(_material);
			m_sizeTexture = SkinManager::getMaterialSize(_material);
			m_num = (size_t)-1;
		}

		inline void setImageRect(const FloatRect & _rect)
		{
			m_rectImage = _rect;
			m_num = (size_t)-1;
		}

		inline void setImageTile(const FloatSize & _tile)
		{
			m_sizeTile = _tile;
			m_num = (size_t)-1;
		}

		void setImageNum(size_t _num);
		inline size_t getImageNum(size_t _num) {return m_num;}

	private:
		// кусок в текстуре наших картинок
		FloatRect m_rectImage;
		// размер одной картинки
		FloatSize m_sizeTile;
		// размер текстуры
		FloatSize m_sizeTexture;
		// текущая картинка
		size_t m_num;
		// единственный оверлей нашего виджета
		PanelAlphaOverlayElement * m_element;

	}; // class StaticImage : public Widget

	typedef StaticImage * StaticImagePtr;

} // namespace MyGUI

#endif 
