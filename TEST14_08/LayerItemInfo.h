#pragma once

#include <Ogre.h>

namespace widget
{

	class LayerInfo;
	class LayerManager;
	class Gui;

	typedef LayerInfo * LayerInfoPtr;

	// �������������� ��������� ��� ���������� �������� ��������
	class LayerItemInfo
	{
		// ��� �������
		friend LayerInfo;
		friend LayerManager;
		friend Gui;

	private:
		Ogre::Overlay * m_overlayInfo;
		LayerInfoPtr m_layerInfo;
	};

	typedef LayerItemInfo * LayerItemInfoPtr;

} // namespace widget