
#include "SubSkin.h"
#include "BasisWidgetManager.h"

namespace widget
{

	// создаем фабрику для этого скина
	BasisWidgetFactory<SubSkin> factory_subSkin;


	SubSkin::SubSkin(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.aligin, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<PanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
			"PanelAlpha", "SubSkin_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_parent->left() + m_x, m_parent->top() + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(_material);

		m_parent->attach(m_overlayContainer, false);
	}

	SubSkin::~SubSkin()
	{
		if (!m_overlayContainer) return;
//		Ogre::OverlayContainer * parent = m_overlayContainer->getParent();
//		parent->removeChild(m_overlayContainer->getName());
		Ogre::OverlayManager::getSingleton().destroyOverlayElement(m_overlayContainer);
	}

	void SubSkin::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		m_show ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 color[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		m_overlayContainer->setColor(*(Ogre::uint32*)color);
	}

	void SubSkin::attach(OverlayElementPtr _element, bool _child)
	{
		m_overlayContainer->addChild(_element);
	}

	void SubSkin::update()
	{

		bool margin = check_margin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = m_x + m_parent->left() - m_parent->getParent()->margin_left() + m_left_margin;
		if (x < 0) x = 0;
		int y = m_y + m_parent->top() - m_parent->getParent()->margin_top() + m_top_margin;
		if (y < 0) y = 0;

		m_overlayContainer->setPosition(x, y);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (check_outside()) {

				// скрываем
				m_overlayContainer->setTransparent(true);
				// запоминаем текущее состояние
				m_margin = margin;

				return;

			}

		}
		
		if ((m_margin) || (margin)) { // мы обрезаны или были обрезаны

			int cx = view_width();
			if (cx < 0) cx = 0;
			int cy = view_height();
			if (cy < 0) cy = 0;

			m_overlayContainer->setDimensions(cx, cy);

			if (cx && cy) {

				// теперь смещаем текстуру
				float UV_lft = m_left_margin;
				float UV_top = m_top_margin;
				float UV_rgt = m_cx - m_right_margin;
				float UV_btm = m_cy - m_bottom_margin;

				UV_lft = UV_lft / (float)m_cx;
				UV_top = UV_top / (float)m_cy;
				UV_rgt = UV_rgt / (float)m_cx;
				UV_btm = UV_btm / (float)m_cy;

				float UV_sizeX = m_rectTexture.right - m_rectTexture.left;
				float UV_sizeY = m_rectTexture.bottom - m_rectTexture.top;

				float UV_lft_total = m_rectTexture.left + UV_lft * UV_sizeX;
				float UV_top_total = m_rectTexture.top + UV_top * UV_sizeY;
				float UV_rgt_total = m_rectTexture.right - (1-UV_rgt) * UV_sizeX;
				float UV_btm_total = m_rectTexture.bottom - (1-UV_btm) * UV_sizeY;

				m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

			}

		}

		// запоминаем текущее состояние
		m_margin = margin;
		// если скин был скрыт, то покажем
		m_overlayContainer->setTransparent(false);

	}

	void SubSkin::correct()
	{
		m_overlayContainer->setPosition(m_x + m_parent->left() - m_parent->getParent()->margin_left() + m_left_margin, m_y + m_parent->top() - m_parent->getParent()->margin_top() + m_top_margin);
	}

	void SubSkin::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void SubSkin::align(int _cx, int _cy, bool _update)
	{

		bool need_update = _update;

		// первоначальное выравнивание 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// растягиваем
				m_cx = m_cx + (m_parent->width() - _cx);
				need_update = true;
				m_margin = true; // при изменении размеров все пересчитывать
			} else {
				// двигаем по правому краю
				m_x = m_x + (m_parent->width() - _cx);
				need_update = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			m_x = (m_parent->width() - m_cx) / 2;
			need_update = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// растягиваем
				m_cy = m_cy + (m_parent->height() - _cy);
				need_update = true;
				m_margin = true; // при изменении размеров все пересчитывать
			} else {
				m_y = m_y + (m_parent->height() - _cy);
				need_update = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			m_y = (m_parent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

	void SubSkin::setUVSet(const Ogre::FloatRect & _rect)
	{
		assert(m_overlayContainer);
		m_rectTexture = _rect;
		// если обрезаны, то просчитываем с учето обрезки
		if (m_margin) {

			float UV_lft = m_left_margin;
			float UV_top = m_top_margin;
			float UV_rgt = m_cx - m_right_margin;
			float UV_btm = m_cy - m_bottom_margin;

			UV_lft = UV_lft / (float)m_cx;
			UV_top = UV_top / (float)m_cy;
			UV_rgt = UV_rgt / (float)m_cx;
			UV_btm = UV_btm / (float)m_cy;

			float UV_sizeX = m_rectTexture.right - m_rectTexture.left;
			float UV_sizeY = m_rectTexture.bottom - m_rectTexture.top;

			float UV_lft_total = m_rectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = m_rectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = m_rectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = m_rectTexture.bottom - (1-UV_btm) * UV_sizeY;

			m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		} else {
			// мы не обрезаны, базовые координаты
			m_overlayContainer->setUV(m_rectTexture.left, m_rectTexture.top, m_rectTexture.right, m_rectTexture.bottom);
		}
	}

} // namespace widget