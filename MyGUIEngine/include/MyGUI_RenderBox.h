/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#ifndef __MYGUI_RENDER_BOX_H__
#define __MYGUI_RENDER_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_FrameListener.h"

namespace MyGUI
{

	class _MyGUIExport RenderBox : public Widget, public FrameListener
	{
		// дл€ вызова закрытого конструктора
		friend class factory::RenderBoxFactory;

	protected:
		RenderBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		virtual ~RenderBox();

	public:
		// тип данного виджета
		inline static const Ogre::String & _getType() {static Ogre::String type("RenderBox"); return type;}
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Add mesh to scene and remove previous one
			@param
				_meshName The name of the Mesh it is to be based on (e.g. 'knot.oof'). The
				mesh will be loaded if it is not already.
		*/
		void injectObject(const Ogre::String& _meshName);

		/** Clear scene */
		void clear();

		/** Set speed of entity rotation.
			@param
				speed of rotation in degrees per second. If 0 then turn rotation off. If 1 then set defaul rotation speed.
		*/
		void setAutoRotate(int _speed = 1);

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

	protected:
		void _frameEntered(float _time);

	private:
		void createRenderMaterial();
		void updateViewport();

		Ogre::SceneManager * mScene;
		Ogre::Entity * mEntity;
		Ogre::SceneNode * mNode;
		Ogre::String mMaterial;
		Ogre::RenderTexture * mTexture;

		Ogre::Camera* mRttCam;
		Ogre::SceneNode* mCamNode;

		// € там чудок накос€чил, измени управление напр€мую оверлеем
		// на класс кроппед, методы € добавил, дл€ примера посмотри статик имадж
		PanelAlphaOverlayElement * mElement;

		bool mFrameListener;

		int mRotationSpeed;
	}; // class RenderBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_H__