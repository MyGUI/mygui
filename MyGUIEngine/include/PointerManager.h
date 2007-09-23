#ifndef _POINTERMANAGER_H_
#define _POINTERMANAGER_H_

#include "Platform.h"
//#include <Ogre.h>
#include "Instance.h"
#include <OgrePanelOverlayElement.h>
#include <string>
//#include "Instance.h"
#include "LayerManager.h"
#include "PointerInfo.h"

namespace MyGUI
{

	class _MyGUIExport PointerManager : /*public Instance<PointerManager>, */public LayerItemInfo
	{
		INSTANCE_HEADER(PointerManager);

	public:
		PointerManager();

		void load(const std::string & _file);
		void clear();

		void show(bool _show = true);
		inline void hide() {show(false);}
		inline bool isShow() {return m_show;}

		void move(int _x, int _y);

		void setPointer(const std::string & _name);
		inline void defaultPointer() {setPointer(m_defaultPointer);};

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);

		void shutdown();

	private:
		std::string m_defaultPointer;
		std::string m_layer;
		std::string m_material;
		bool m_show;
		intPoint m_point;
		MapPointerInfo m_mapPointers;
		PanelAlphaOverlayElement * m_overlayElement;

	}; // class PointerManager

} // namespace MyGUI

#endif
