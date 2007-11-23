
#include "SubSkin.h"

namespace MyGUI
{

	SubSkin::SubSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<PanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
			"PanelAlpha", "SubSkin_" + Ogre::StringConverter::toString((Ogre::uint32)this)) );

		m_overlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
		m_overlayContainer->setPosition(mParent->left() + m_x, mParent->top() + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		if (false == _material.empty()) m_overlayContainer->setMaterialName(_material);

		mParent->attach(this, false);
	}

	SubSkin::~SubSkin()
	{
		if (m_overlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(m_overlayContainer);
	}

	void SubSkin::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;
		mShow ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 color[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		m_overlayContainer->setColor(*(Ogre::uint32*)color);
	}

	void SubSkin::attach(BasisWidgetPtr _basis, bool _child)
	{
		m_overlayContainer->addChild(_basis->getOverlayElement());
	}

	Ogre::OverlayElement* SubSkin::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void SubSkin::correct()
	{
		// либо просто двигаться, либо с учетом выравнивания отца
		if (mParent->getParent()) m_overlayContainer->setPosition(m_x + mParent->left() - mParent->getParent()->margin_left() + m_left_margin, m_y + mParent->top() - mParent->getParent()->margin_top() + m_top_margin);
		else m_overlayContainer->setPosition(m_x + mParent->left(), m_y + mParent->top());
	}

	void SubSkin::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void SubSkin::align(int _cx, int _cy, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// растягиваем
				m_cx = m_cx + (mParent->width() - _cx);
				need_update = true;
				mMargin = true; // при изменении размеров все пересчитывать
			} else {
				// двигаем по правому краю
				m_x = m_x + (mParent->width() - _cx);
				need_update = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			m_x = (mParent->width() - m_cx) / 2;
			need_update = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// растягиваем
				m_cy = m_cy + (mParent->height() - _cy);
				need_update = true;
				mMargin = true; // при изменении размеров все пересчитывать
			} else {
				m_y = m_y + (mParent->height() - _cy);
				need_update = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			m_y = (mParent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

	void SubSkin::update()
	{

		bool margin = check_margin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = m_x + mParent->left() - (mParent->getParent() ? mParent->getParent()->margin_left() : 0) + m_left_margin;
//		if (x < 0) x = 0;
		int y = m_y + mParent->top() - (mParent->getParent() ? mParent->getParent()->margin_top() : 0) + m_top_margin;
//		if (y < 0) y = 0;

		m_overlayContainer->setPosition(x, y);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (check_outside()) {

				// скрываем
				m_overlayContainer->setTransparent(true);
				// запоминаем текущее состояние
				mMargin = margin;

				return;

			}

		}
		
		if ((mMargin) || (margin)) { // мы обрезаны или были обрезаны

			int cx = view_width();
			if (cx < 0) cx = 0;
			int cy = view_height();
			if (cy < 0) cy = 0;

			m_overlayContainer->setDimensions(cx, cy);

			if (cx && cy) {

				// теперь смещаем текстуру
				float UV_lft = m_left_margin / (float)m_cx;
				float UV_top = m_top_margin / (float)m_cy;
				float UV_rgt = (m_cx - m_right_margin) / (float)m_cx;
				float UV_btm = (m_cy - m_bottom_margin) / (float)m_cy;

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
		mMargin = margin;
		// если скин был скрыт, то покажем
		m_overlayContainer->setTransparent(false);

	}

	void SubSkin::setUVSet(const FloatRect & _rect)
	{
		assert(m_overlayContainer);
		m_rectTexture = _rect;
		// если обрезаны, то просчитываем с учето обрезки
		if (mMargin) {

			float UV_lft = m_left_margin / (float)m_cx;
			float UV_top = m_top_margin / (float)m_cy;
			float UV_rgt = (m_cx - m_right_margin) / (float)m_cx;
			float UV_btm = (m_cy - m_bottom_margin) / (float)m_cy;

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

} // namespace MyGUI