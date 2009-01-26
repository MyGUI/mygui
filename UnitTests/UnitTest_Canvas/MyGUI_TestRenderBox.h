/*!
	@file
	@author		Evmenov Georgiy
	@author		Alexander Ptakhin
	@date		01/2009
	@module
*/
#ifndef __MYGUI_TEST_RENDER_BOX_H__
#define __MYGUI_TEST_RENDER_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Canvas.h"

namespace Ogre
{
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
	typedef TRect<Real> RealRect;
#endif
}

namespace MyGUI
{
	namespace factory
	{
		class TestRenderBoxFactory;
	}

	class TestRenderBox;

	typedef TestRenderBox* TestRenderBoxPtr;

	/** @brief Widget that show one entity or anything from viewport.

		This widget can show autorotaded and rotatable by mouse mesh.
		Also you can set your own Ogre::Camera and yo'll see anything from your viewport.
	*/
	class /*MYGUI_EXPORT*/ TestRenderBox : public  Canvas
	{
		// для вызова закрытого конструктора
		friend class factory::TestRenderBoxFactory;

		MYGUI_RTTI_CHILD_HEADER( TestRenderBox, Widget );

	public:
		/** default TestRenderBox autorotation speed (if enabled) */
		enum { RENDER_BOX_AUTO_ROTATION_SPEED = 20 };

		/** Set any user created Camera instead of showing one mesh*/
		void setCamera(Ogre::Camera * _camera);

		/** Removes camera. Renders nothing */
		void removeCamera( /*bool _clearbool _deleteTexture=false */);

		Ogre::Viewport* getViewport() { return mViewport; }

		bool getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen);

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

	protected:
		TestRenderBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~TestRenderBox();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		void preTextureChanges( MyGUI::CanvasPtr _canvas );

		void requestUpdateCanvas( MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _canvasEvent );

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void frameEntered(float _time);

		bool needFrameUpdate() {return true;}
		//void createRenderTexture();
		void updateViewport();

		// все, что касается сцены
		Ogre::RenderTexture* mRenderTexture;

		Ogre::Camera* mRttCam;
		Ogre::Viewport* mViewport;

	}; // class TestRenderBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_TEST_RENDER_BOX_H__
