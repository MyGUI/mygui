#ifndef _LAYERITEMINFO_H_
#define _LAYERITEMINFO_H_

#include "Prerequest.h"
#include <Ogre.h>

namespace widget
{

	class LayerInfo;
	class LayerItemInfo;
	class LayerManager;

	typedef LayerInfo * LayerInfoPtr;
	typedef LayerItemInfo * LayerItemInfoPtr;

	// вспомогательня структура для управления уровнями оверлеев
	class LayerItemInfo
	{
		// для доступа
		friend LayerInfo;
		friend LayerManager;

	protected:
		LayerItemInfo() : m_overlayInfo(0), m_layerInfo(0) {}

		// для виджетов нуно это добро переопределить
		virtual LayerItemInfoPtr findItem(int _x, int _y) {return 0;}

		// это нужно для подсоединения к оверлею
		virtual void attachToOverlay(Ogre::Overlay * _overlay) = 0;
		virtual void detachToOverlay(Ogre::Overlay * _overlay) = 0;

	private:
		Ogre::Overlay * m_overlayInfo;
		LayerInfoPtr m_layerInfo;
	};

} // namespace widget

#endif
