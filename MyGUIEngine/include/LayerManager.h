#ifndef _LAYERMANAGER_H_
#define _LAYERMANAGER_H_

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

	class _MyGUIExport LayerManager : public Instance<LayerManager>
	{
		friend Gui;

		INSTANCE_HEADER(LayerManager);

	public:

		void attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch = false);
		void detachItem(LayerItemInfoPtr _item);
		void upItem(LayerItemInfoPtr _item);

		LayerItemInfoPtr findWidgetItem(int _x, int _y, LayerItemInfoPtr & _rootItem);

		Ogre::Overlay * createOverlay();

		~LayerManager();

		bool load(const std::string & _file);
		void clear();

	private:

		LayerManager();

	private:
		MapLayer m_mapLayer;
		MapLayerSearch m_mapLayerSearch;

	}; // class LayerManager

} // namespace MyGUI

#endif