#pragma once

#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include <string>
#include "Instance.h"
#include "LayerManager.h"
#include "PointerInfo.h"

namespace widget
{

	class PointerManager : public LayerItemInfo
	{
	private:
		PointerManager();
	public:
		INSTANCE(PointerManager)

		void load(const std::string & _file);
		void clear();

		void show(bool _show = true);
		inline void hide() {show(false);}
		inline bool isShow() {return m_show;}

		void move(int _x, int _y);

		void setPointer(const std::string & _name);
		inline void defaultPointer() {setPointer(m_defaultPointer);};

	private:
		std::string m_defaultPointer;
		std::string m_layer;
		std::string m_material;
		bool m_show;
		intPoint m_point;
		MapPointerInfo m_mapPointers;
		Ogre::PanelOverlayElement * m_overlayElement;

	}; // class PointerManager

} // namespace widget