/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SUB_SKIN_H__
#define __MYGUI_SUB_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SharedPanelAlphaOverlayElement.h"
#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	class _MyGUIExport SubSkin : public CroppedRectangleInterface
	{

	public:
		SubSkin(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id);
		virtual ~SubSkin();

		void setAlpha(float _alpha);

		void show();
		void hide();

		void _updateView(); // обновления себя и детей
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void _setUVSet(const FloatRect& _rect);

		void _attachChild(CroppedRectanglePtr _basis, bool _child);
		Ogre::OverlayElement* _getOverlayElement();

		inline static const Ogre::String & _getType() {static Ogre::String type("SubSkin"); return type;}
		inline static bool _isSharedOverlay() {return true;}
		bool _isText() {return false;}

		Ogre::OverlayElement* _getSharedOverlayElement() {return (mId == 0) ? mOverlayContainer : null;}

	protected:

		inline void _setTransparent(bool _transparent)
		{
			if (mTransparent == _transparent) return;
			mTransparent = _transparent;
			mOverlayContainer->setTransparentInfo(mTransparent, mId);
		}

	protected:

		SharedPanelAlphaOverlayElement * mOverlayContainer;
		FloatRect mRectTexture;
		size_t mId;
		bool mTransparent;

	}; // class _MyGUIExport SubSkin : public SubWidgetSkinInterface

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
