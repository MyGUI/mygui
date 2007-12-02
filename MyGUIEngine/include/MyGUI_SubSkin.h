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
#include "MyGUI_BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport SubSkin : public BasisWidget
	{

	public:
		SubSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent, size_t _id);
		virtual ~SubSkin();

		void show();
		void hide();

		void update(); // обновления себя и детей
		void correct();

		void align(int _width, int _height, bool _update);
		void align(int _left, int _top, int _width, int _height, bool _update);

		void setUVSet(const FloatRect & _rect);

		void setAlpha(float _alpha);

		void attach(BasisWidgetPtr _basis, bool _child);
		Ogre::OverlayElement* getOverlayElement();

		inline static const Ogre::String & getType() {static Ogre::String type("SubSkin"); return type;}
		inline static bool isSharedOverlay() {return true;}
		bool isText() {return false;}

		Ogre::OverlayElement* _getSharedOverlay() {return (mId == 0) ? mOverlayContainer : null;}

	protected:

		inline void _setTransparent(bool _transparent)
		{
			if (mTransparent == _transparent) return;
			mTransparent = _transparent;
			if ((false == mTransparent) && (false == mShow)) return;
			mOverlayContainer->setTransparentInfo(mTransparent, mId);
		}

	protected:

		SharedPanelAlphaOverlayElement * mOverlayContainer;
		FloatRect mRectTexture;
		size_t mId;
		bool mTransparent;

	}; // class SubSkin

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__