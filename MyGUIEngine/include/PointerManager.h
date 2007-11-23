#ifndef __POINTER_MANAGER_H__
#define __POINTER_MANAGER_H__

#include "Prerequest.h"
#include "Platform.h"
#include "Instance.h"
#include <OgrePanelOverlayElement.h>
#include <string>
#include "LayerManager.h"
#include "PointerInfo.h"

namespace MyGUI
{

	class _MyGUIExport PointerManager : public LayerItemInfo
	{
		INSTANCE_HEADER(PointerManager);

	public:
		void initialise();
		void shutdown();

	public:

		void load(const std::string & _file);
		void clear();

		void show(bool _show = true);
		inline void hide() {show(false);}
		inline bool isShow() {return mShow;}

		void move(int _x, int _y);

		void setPointer(const std::string & _name);
		inline void defaultPointer() {setPointer(m_defaultPointer);};

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);

	private:
		std::string m_defaultPointer;
		std::string m_layer;
		std::string m_material;
		bool mShow;
		IntPoint m_point;
		MapPointerInfo m_mapPointers;
		PanelAlphaOverlayElement * m_overlayElement;

	}; // class PointerManager

} // namespace MyGUI

#endif // __POINTER_MANAGER_H__
