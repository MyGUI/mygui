#ifndef _LAYERMANAGER_H_
#define _LAYERMANAGER_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include <string>
#include <map>
#include "LayerInfo.h"
#include "xmlDocument.h"
#include "WidgetDefines.h"
#include "Instance.h"


namespace MyGUI
{
	
	// карта для поиска по имени
	typedef std::map<std::string, LayerInfoPtr> MapLayer;
	// карта для упорядочивания по высоте
	typedef std::map<Ogre::ushort, LayerInfoPtr> MapLayerSearch;

	class _MyGUIExport LayerManager
	{
		friend Gui;

	public:
		INSTANCE(LayerManager)

		void attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch = false);
		void detachItem(LayerItemInfoPtr _item);
		void upItem(LayerItemInfoPtr _item);

		LayerItemInfoPtr findItem(int _x, int _y);

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