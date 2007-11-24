#ifndef __LAYER_MANAGER_H__
#define __LAYER_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "LayerInfo.h"
#include "xmlDocument.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	
	// ����� ��� ������ �� �����
	typedef std::map<std::string, LayerInfoPtr> MapLayer;
	// ����� ��� �������������� �� ������
	typedef std::map<Ogre::ushort, LayerInfoPtr> MapLayerSearch;

	class _MyGUIExport LayerManager
	{
		INSTANCE_HEADER(LayerManager);

	public:
		inline void initialise() {}
		inline void shutdown() {clear();}

		void attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch = false);
		void detachItem(LayerItemInfoPtr _item);
		void upItem(LayerItemInfoPtr _item);

		LayerItemInfoPtr findWidgetItem(int _left, int _top, LayerItemInfoPtr & _rootItem);

		Ogre::Overlay * createOverlay();

		bool load(const std::string & _file);
		void clear();

	private:
		MapLayer mMapLayer;
		MapLayerSearch mMapLayerSearch;

	}; // class LayerManager

} // namespace MyGUI

#endif // __LAYER_MANAGER_H__