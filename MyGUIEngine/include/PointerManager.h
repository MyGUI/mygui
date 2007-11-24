#ifndef __POINTER_MANAGER_H__
#define __POINTER_MANAGER_H__

#include "Prerequest.h"
#include "Platform.h"
#include "Instance.h"
#include <OgrePanelOverlayElement.h>
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

		void move(int _left, int _top);

		void setPointer(const std::string & _name);
		inline void defaultPointer() {setPointer(mDefaultPointer);};

		void attachToOverlay(Ogre::Overlay * _overlay);
		void detachToOverlay(Ogre::Overlay * _overlay);

	private:
		std::string mDefaultPointer;
		std::string mLayer;
		std::string mMaterial;
		bool mShow;
		IntPoint mPoint;
		MapPointerInfo mMapPointers;
		PanelAlphaOverlayElement * mOverlayElement;

	}; // class PointerManager

} // namespace MyGUI

#endif // __POINTER_MANAGER_H__
