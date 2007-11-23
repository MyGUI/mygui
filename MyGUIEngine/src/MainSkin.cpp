
#include "MainSkin.h"

namespace MyGUI
{

	MainSkin::MainSkin(const BasisWidgetInfo& _info, const Ogre::String& _material, BasisWidgetPtr _parent) : 
	BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{

		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<PanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
			"PanelAlpha", "MainSkin_" + Ogre::StringConverter::toString((Ogre::uint32)this)) );

		m_overlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
		m_overlayContainer->setPosition(mParent->left() + m_x, mParent->top() + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		if (false == _material.empty() && (_info.offset.width() != 0)) m_overlayContainer->setMaterialName(_material);

		mParent->attach(this, false);
	}

	MainSkin::~MainSkin()
	{
		if (m_overlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(m_overlayContainer);
	}

	void MainSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 color[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		m_overlayContainer->setColor(*(Ogre::uint32*)color);
	}

	void MainSkin::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;
		_show ? m_overlayContainer->show() : m_overlayContainer->hide();
	};

	void MainSkin::align(int _cx, int _cy, bool _update)
	{

		if (_update) {
			update();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(mParent->width() < 0) ? m_cx = 0 : m_cx = mParent->width();
		(mParent->height() < 0) ? m_cy = 0 : m_cy = mParent->height();
		m_overlayContainer->setDimensions(m_cx, m_cy);

	}

	void MainSkin::align(int _x, int _y, int _cx, int _cy, bool _update)
	{

		m_overlayContainer->setPosition(m_x + mParent->left() - mParent->getParent()->margin_left(), m_y + mParent->top() - mParent->getParent()->margin_top());

		if (_update) {
			update();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(mParent->width() < 0) ? m_cx = 0 : m_cx = mParent->width();
		(mParent->height() < 0) ? m_cy = 0 : m_cy = mParent->height();
		m_overlayContainer->setDimensions(m_cx, m_cy);

	}

	void MainSkin::correct()
	{
		// либо просто двигаться, либо с учетом выравнивания отца
		if (mParent->getParent()) m_overlayContainer->setPosition(m_x + mParent->left() - mParent->getParent()->margin_left() + m_left_margin, m_y + mParent->top() - mParent->getParent()->margin_top() + m_top_margin);
		else m_overlayContainer->setPosition(m_x + mParent->left(), m_y + mParent->top());
	}

	void MainSkin::update()
	{
		int cx = mParent->view_width();
		if (cx < 0) cx = 0;
		int cy = mParent->view_height();
		if (cy < 0) cy = 0;

		//порубали оверлей
		m_overlayContainer->setPosition(mParent->view_left() - (mParent->getParent() ? mParent->getParent()->margin_left() : 0), mParent->view_top() - (mParent->getParent() ? mParent->getParent()->margin_top() : 0) );
		m_overlayContainer->setDimensions(cx, cy);

		// теперь смещаем текстуру
		float UV_lft = mParent->margin_left() / (float)mParent->width();
		float UV_top = mParent->margin_top() / (float)mParent->height();
		float UV_rgt = (mParent->width() - mParent->margin_right()) / (float)mParent->width();
		float UV_btm = (mParent->height() - mParent->margin_bottom()) / (float)mParent->height();

		float UV_sizeX = m_rectTexture.right - m_rectTexture.left;
		float UV_sizeY = m_rectTexture.bottom - m_rectTexture.top;

		float UV_lft_total = m_rectTexture.left + UV_lft * UV_sizeX;
		float UV_top_total = m_rectTexture.top + UV_top * UV_sizeY;
		float UV_rgt_total = m_rectTexture.right - (1-UV_rgt) * UV_sizeX;
		float UV_btm_total = m_rectTexture.bottom - (1-UV_btm) * UV_sizeY;

		m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

	}

	void MainSkin::attach(BasisWidgetPtr _basis, bool _child)
	{
		m_overlayContainer->addChild(_basis->getOverlayElement());
	}

	Ogre::OverlayElement* MainSkin::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void MainSkin::setUVSet(const FloatRect & _rect)
	{
		assert(m_overlayContainer);
		m_rectTexture = _rect;
		// если обрезаны, то просчитываем с учето обрезки
		if (mMargin) {

			float UV_lft = mParent->margin_left() / (float)mParent->width();
			float UV_top = mParent->margin_top() / (float)mParent->height();
			float UV_rgt = (mParent->width() - mParent->margin_right()) / (float)mParent->width();
			float UV_btm = (mParent->height() - mParent->margin_bottom()) / (float)mParent->height();

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


} // namespace MainSkin