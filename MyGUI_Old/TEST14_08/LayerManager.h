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
	// ����� ��� ������ �� �����
	typedef std::map<std::string, LayerInfoPtr> MapLayer;
	// ����� ��� �������������� �� ������
	typedef std::map<Ogre::ushort, LayerInfoPtr> MapLayerSearch;

	class LayerManager
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

} // namespace widget