/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_INFO_H__
#define __MYGUI_LAYER_ITEM_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerItemKeeper.h"

#include <Ogre.h>

namespace MyGUI
{

	class LayerInfo;
	class LayerItemInfo;
	class LayerManager;

	typedef LayerInfo * LayerInfoPtr;
	typedef LayerItemInfo * LayerItemInfoPtr;
	typedef std::vector<LayerItemInfoPtr> VectorLayerItemInfo;

	// �������������� ��������� ��� ���������� �������� ��������
	class _MyGUIExport LayerItemInfo
	{
		// ��� �������
		friend class LayerInfo;
		friend class LayerManager;

    public:
		virtual ~LayerItemInfo();

	protected:
		LayerItemInfo();

		// ��� �������� ���� ��� ����� ��������������
		virtual LayerItemInfoPtr findItem(int _left, int _top) = 0;

		// ��� ����� ��� ������������� � �������
		virtual void attachToOverlay(Ogre::Overlay * _overlay) = 0;
		virtual void detachToOverlay(Ogre::Overlay * _overlay) = 0;

	protected:
		Ogre::Overlay * mOverlayInfo;
		LayerItemKeeper * mLayerItemKeeperInfo;
		LayerInfoPtr mLayerInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_INFO_H__
