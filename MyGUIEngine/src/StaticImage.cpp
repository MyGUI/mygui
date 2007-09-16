
#include "StaticImage.h"
#include "StaticImageParser.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<StaticImage> StaticImageFactoryInstance("StaticImage"); }
	// ������ ������
	namespace parser { StaticImageParser StaticImageParserInstance; }

	StaticImage::StaticImage(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_num(0)
	{
		// �������������� �������������
		ASSERT(m_subSkinChild.size() == 1);
		m_element = static_cast<PanelAlphaOverlayElement *>(m_subSkinChild[0]->getOverlayElement());
		ASSERT(m_element);

		// ������ ��������
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("ImageMaterial");
			if (iter != param.end()) setImageMaterial(iter->second);
			iter = param.find("ImageRect");
			if (iter != param.end()) setImageRect(floatRect::parse(iter->second));
			iter = param.find("ImageTile");
			if (iter != param.end()) setImageTile(floatSize::parse(iter->second));
			iter = param.find("ImageNum");
			if (iter != param.end()) setImageNum(parseInt(iter->second));
		}
	}

	void StaticImage::setImageNum(size_t _num)
	{
		if (_num == m_num) return;
		m_num = _num;

		// ���� ������ �������, �� ������ ������ ��������
		if (!(m_rectImage.right || m_rectImage.bottom)) {
			m_rectImage.right = m_sizeTexture.width;
			m_rectImage.bottom = m_sizeTexture.width;
		}

		size_t count = (size_t)(m_rectImage.width() / m_sizeTile.width);
		if (count < 1) count = 1;

		floatRect offset(
			((float)(m_num % count)) * m_sizeTile.width + m_rectImage.left,
			((float)(m_num / count)) * m_sizeTile.height + m_rectImage.top,
			m_sizeTile.width, m_sizeTile.height);
		// ������������ � �������������
		offset = SkinManager::convertMaterialCoord(offset, m_sizeTexture);
		m_element->setUV(offset.left, offset.top, offset.right, offset.bottom);
	}


} // namespace MyGUI