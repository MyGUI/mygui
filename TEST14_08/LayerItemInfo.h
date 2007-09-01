#pragma once

#include <Ogre.h>

namespace widget
{

	class LayerInfo;
	class LayerItemInfo;
	class LayerManager;
	class Gui;

	typedef LayerInfo * LayerInfoPtr;
	typedef LayerItemInfo * LayerItemInfoPtr;

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

		// для виджетов нуно это добро переопределить
		virtual LayerItemInfoPtr findItem(int _x, int _y) {return 0;}

	private:
		Ogre::Overlay * m_overlayInfo;
		LayerInfoPtr m_layerInfo;
	};

} // namespace widget