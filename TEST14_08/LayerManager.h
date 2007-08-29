#pragma once

#include <Ogre.h>
#include <string>
#include <map>
#include "LayerInfo.h"
#include "xmlDocument.h"
#include "Widget.h"
#include "Instance.h"


namespace widget
{
	class Gui;
	typedef std::map<std::string, LayerInfoPtr> MapLayer;

	class LayerManager
	{
		// кроме гуя некому рулить уровнями
		friend Gui;

	private:
		INSTANCE(LayerManager)

		LayerManager();


		bool load(const std::string & _file);
		void clear();

		/*inline */void attachWidget(WidgetPtr _widget, const std::string & _layer);
		/*inline*/ void detachWidget(WidgetPtr _widget);
		/*inline */void upWidget(WidgetPtr _widget);

	private:
		MapLayer m_mapLayer;

	}; // class LayerManager

} // namespace widget