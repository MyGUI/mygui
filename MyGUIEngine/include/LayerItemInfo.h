#ifndef __LAYER_ITEM_INFO_H__
#define __LAYER_ITEM_INFO_H__

#include "Prerequest.h"

namespace MyGUI
{

	class LayerInfo;
	class LayerItemInfo;
	class LayerManager;

	typedef LayerInfo * LayerInfoPtr;
	typedef LayerItemInfo * LayerItemInfoPtr;
	typedef std::vector<LayerItemInfoPtr> VectorLayerItemInfo;

	// вспомогательня структура для управления уровнями оверлеев
	class LayerItemInfo
	{
		// для доступа
		friend LayerInfo;
		friend LayerManager;

	protected:
		LayerItemInfo() : mOverlayInfo(0), mLayerInfo(0) {}

		// для виджетов нуно это добро переопределить
		virtual LayerItemInfoPtr findItem(int _left, int _top) {return 0;}

		// это нужно для подсоединения к оверлею
		virtual void attachToOverlay(Ogre::Overlay * _overlay) = 0;
		virtual void detachToOverlay(Ogre::Overlay * _overlay) = 0;

		// для виджетов переопределить, для остальных низя
		virtual bool isWidget() {return false;}

	private:
		Ogre::Overlay * mOverlayInfo;
		LayerInfoPtr mLayerInfo;
	};

} // namespace MyGUI

#endif // __LAYER_ITEM_INFO_H__