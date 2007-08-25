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

	private:
		Ogre::Overlay * m_overlayInfo;
		LayerInfoPtr m_layerInfo;
	};

	typedef LayerItemInfo * LayerItemInfoPtr;

} // namespace widget