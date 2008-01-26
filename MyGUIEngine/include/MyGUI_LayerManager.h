/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LAYER_MANAGER_H__
#define __MYGUI_LAYER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_LayerInfo.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_UnlinkWidget.h"

namespace MyGUI
{
	
	// карта для поиска по имени
	typedef std::map<std::string, LayerInfoPtr> MapLayer;
	// карта для упорядочивания по высоте
	typedef std::map<Ogre::ushort, LayerInfoPtr> MapLayerSearch;

	class _MyGUIExport LayerManager : public UnlinkWidget
	{
		INSTANCE_HEADER(LayerManager);

	public:
		void initialise();
		void shutdown();

		void attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch = false);
		void detachItem(LayerItemInfoPtr _item);
		void upItem(LayerItemInfoPtr _item);

		LayerItemInfoPtr findWidgetItem(int _left, int _top, LayerItemInfoPtr & _rootItem);

		Ogre::Overlay * createOverlay();

		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void clear();

		void _load(xml::xmlNodePtr _node, const std::string & _file);
	private:
		void _unlinkWidget(WidgetPtr _widget);

	private:
		MapLayer mMapLayer;
		MapLayerSearch mMapLayerSearch;

	}; // class LayerManager

} // namespace MyGUI

#endif // __MYGUI_LAYER_MANAGER_H__
