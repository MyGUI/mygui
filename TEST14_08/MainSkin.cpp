
#include "MainSkin.h"
//#include "BasisWidgetFactory.h"
#include "BasisWidgetManager.h"

namespace widget
{

	// ������� ������� ��� ����� �����
	BasisWidgetFactory<MainSkin> factory_mainSkin;

	MainSkin::MainSkin(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent) : 
	BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.aligin, _parent)
	{

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<PanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
			"PanelAlpha", "MainSkin_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_parent->left() + m_x, m_parent->top() + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(_material);

		m_parent->attach(this, false);
	}

	MainSkin::~MainSkin()
	{
		if (m_overlayContainer == null) return;
		// � ������� �� �������� ����� �������� �������
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
		if (m_show == _show) return;
		m_show = _show;
		_show ? m_overlayContainer->show() : m_overlayContainer->hide();
	};

	void MainSkin::align(int _cx, int _cy, bool _update)
	{

		if (_update) {
			update();
			return;
		}

		// ���� ��������� �� ����, �� ������ ������ ������
		(m_parent->width() < 0) ? m_cx = 0 : m_cx = m_parent->width();
		(m_parent->height() < 0) ? m_cy = 0 : m_cy = m_parent->height();
		m_overlayContainer->setDimensions(m_cx, m_cy);

	}

	void MainSkin::align(int _x, int _y, int _cx, int _cy, bool _update)
	{

		m_overlayContainer->setPosition(m_x + m_parent->left() - m_parent->getParent()->margin_left(), m_y + m_parent->top() - m_parent->getParent()->margin_top());

		if (_update) {
			update();
			return;
		}

		// ���� ��������� �� ����, �� ������ ������ ������
		(m_parent->width() < 0) ? m_cx = 0 : m_cx = m_parent->width();
		(m_parent->height() < 0) ? m_cy = 0 : m_cy = m_parent->height();
		m_overlayContainer->setDimensions(m_cx, m_cy);

	}

	void MainSkin::correct()
	{
		// ���� ������ ���������, ���� � ������ ������������ ����
		if (m_parent->getParent()) m_overlayContainer->setPosition(m_x + m_parent->left() - m_parent->getParent()->margin_left() + m_left_margin, m_y + m_parent->top() - m_parent->getParent()->margin_top() + m_top_margin);
		else m_overlayContainer->setPosition(m_x + m_parent->left(), m_y + m_parent->top());
	}

	void MainSkin::update()
	{
		int cx = m_parent->view_width();
		if (cx < 0) cx = 0;
		int cy = m_parent->view_height();
		if (cy < 0) cy = 0;

		//�������� �������
		m_overlayContainer->setPosition(m_parent->view_left() - (m_parent->getParent() ? m_parent->getParent()->margin_left() : 0), m_parent->view_top() - (m_parent->getParent() ? m_parent->getParent()->margin_top() : 0) );
		m_overlayContainer->setDimensions(cx, cy);

		// ������ ������� ��������
		float UV_lft = m_parent->margin_left() / (float)m_parent->width();
		float UV_top = m_parent->margin_top() / (float)m_parent->height();
		float UV_rgt = (m_parent->width() - m_parent->margin_right()) / (float)m_parent->width();
		float UV_btm = (m_parent->height() - m_parent->margin_bottom()) / (float)m_parent->height();

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

	OverlayElementPtr MainSkin::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void MainSkin::setUVSet(const floatRect & _rect)
	{
		assert(m_overlayContainer);
		m_rectTexture = _rect;
		// ���� ��������, �� ������������ � ����� �������
		if (m_margin) {

			float UV_lft = m_parent->margin_left() / (float)m_parent->width();
			float UV_top = m_parent->margin_top() / (float)m_parent->height();
			float UV_rgt = (m_parent->width() - m_parent->margin_right()) / (float)m_parent->width();
			float UV_btm = (m_parent->height() - m_parent->margin_bottom()) / (float)m_parent->height();

			float UV_sizeX = m_rectTexture.right - m_rectTexture.left;
			float UV_sizeY = m_rectTexture.bottom - m_rectTexture.top;

			float UV_lft_total = m_rectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = m_rectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = m_rectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = m_rectTexture.bottom - (1-UV_btm) * UV_sizeY;

			m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		} else {
			// �� �� ��������, ������� ����������
			m_overlayContainer->setUV(m_rectTexture.left, m_rectTexture.top, m_rectTexture.right, m_rectTexture.bottom);
		}
	}


} // namespace MainSkin