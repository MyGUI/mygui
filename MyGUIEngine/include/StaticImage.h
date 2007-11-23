#ifndef __STATIC_IMAGE_H__
#define __STATIC_IMAGE_H__

#include "Prerequest.h"
#include "StaticImageFactory.h"
#include "Widget.h"
#include "PanelAlphaOverlayElement.h"

namespace MyGUI
{

	class _MyGUIExport StaticImage : public Widget
	{
		// ��� ������ ��������� ������������
		friend factory::StaticImageFactory;

	protected:
		StaticImage(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:

		void setImageInfo(const std::string & _material, const FloatSize & _tile);
		void setImageInfo(const std::string & _material, const FloatRect & _rect, const FloatSize & _tile);
		void setImageMaterial(const std::string & _material);

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
		// ����� � �������� ����� ��������
		FloatRect m_rectImage;
		// ������ ����� ��������
		FloatSize m_sizeTile;
		// ������ ��������
		FloatSize m_sizeTexture;
		// ������� ��������
		size_t m_num;
		// ������������ ������� ������ �������
		PanelAlphaOverlayElement * m_element;

	}; // class StaticImage : public Widget

	typedef StaticImage * StaticImagePtr;

} // namespace MyGUI

#endif // __STATIC_IMAGE_H__