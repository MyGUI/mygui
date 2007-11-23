#ifndef __LAYER_MANAGER_H__
#define __LAYER_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include <string>
#include <map>
#include "LayerInfo.h"
#include "xmlDocument.h"
#include "WidgetDefines.h"


namespace MyGUI
{
	
	// карта для поиска по имени
	typedef std::map<std::string, LayerInfoPtr> MapLayer;
	// карта для упорядочивания по высоте
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

		LayerItemInfoPtr findWidgetItem(int _x, int _y, LayerItemInfoPtr & _rootItem);

		Ogre::Overlay * createOverlay();

		bool load(const std::string & _file);
		void clear();

	private:
		MapLayer m_mapLayer;
		MapLayerSearch m_mapLayerSearch;

	}; // class LayerManager

} // namespace MyGUI

#endif // __LAYER_MANAGER_H__