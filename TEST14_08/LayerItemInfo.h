#pragma once

#include <Ogre.h>

namespace widget
{

	class LayerInfo;
	class LayerManager;
	class Gui;

	typedef LayerInfo * LayerInfoPtr;

	// вспомогательня структура для управления уровнями оверлеев
	class LayerItemInfo
	{
		// для доступа
		friend LayerInfo;
		friend LayerManager;
		friend Gui;

	protected:
		LayerItemInfo() :
			m_overlayInfo(0),
			m_layerInfo(0)
		{
		}

		inline void setOverlay(Ogre::Overlay * _overlay)
		{
			if (m_overlayInfo != 0) {
				// отсоединить от менеджера
			}
			m_overlayInfo = _overlay;
		}

		inline void layerItem_attachElement(Ogre::OverlayContainer * _element)
		{
			if (m_overlayInfo) m_overlayInfo->add2D(_element);
		}

	private:
		Ogre::Overlay * m_overlayInfo;
		LayerInfoPtr m_layerInfo;
	};

	typedef LayerItemInfo * LayerItemInfoPtr;

} // namespace widget