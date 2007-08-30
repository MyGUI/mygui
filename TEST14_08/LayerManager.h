#pragma once

#include <Ogre.h>
#include <string>
#include <map>
#include "LayerInfo.h"
#include "xmlDocument.h"
#include "WidgetDefines.h"
#include "Instance.h"


namespace widget
{
	class Gui;
	typedef std::map<std::string, LayerInfoPtr> MapLayer;

	class LayerManager
	{
		friend Gui;

	public:
		INSTANCE(LayerManager)

		void attachItem(LayerItemInfoPtr _item, const std::string & _layer);
		void detachItem(LayerItemInfoPtr _item);
		void upItem(LayerItemInfoPtr _item);

		Ogre::Overlay * createOverlay();

	private:

		LayerManager();

		bool load(const std::string & _file);
		void clear();

	private:
		MapLayer m_mapLayer;

	}; // class LayerManager

} // namespace widget