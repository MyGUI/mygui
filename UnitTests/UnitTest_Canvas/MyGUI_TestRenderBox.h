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
	class /*MYGUI_EXPORT*/ TestRenderBox : public Canvas, Ogre::RenderTargetListener
	{
		// для вызова закрытого конструктора
		friend class factory::TestRenderBoxFactory;

		MYGUI_RTTI_CHILD_HEADER( TestRenderBox, Widget );

	public:
		/** default TestRenderBox autorotation speed (if enabled) */
		enum { RENDER_BOX_AUTO_ROTATION_SPEED = 20 };

		/** Clear scene */
		void clear();


		/** Set colour behind entity.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setBackgroungColour(const Ogre::ColourValue& _backgroundColour);
		/** Get colour behind entity.*/
		const Ogre::ColourValue& getBackgroungColour() { return mBackgroungColour; }

		/** Set possibility to zoom mesh by mouse wheel.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setViewScale(bool _scale);
		/** Get possibility to zoom mesh by mouse wheel flag. */
		bool getViewScale() {return mUseScale;}

		/** Set any user created Camera instead of showing one mesh*/
		void setRenderTarget(Ogre::Camera * _camera);

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

		void onMouseDrag(int _left, int _top);
		void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		void onMouseWheel(int _rel);

		void updateCanvas( MyGUI::CanvasPtr _canvas, MyGUI::CanvasEvent _canvasEvent );

		void preRenderTargetUpdate( const Ogre::RenderTargetEvent & evt );

		void postRenderTargetUpdate( const Ogre::RenderTargetEvent & evt );

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void frameEntered(float _time);

		bool needFrameUpdate() {return true;}
		//void createRenderTexture();
		void updateViewport();

		bool mUserViewport;
		// все, что касается сцены
		Ogre::RenderTexture* mRenderTexture;

		Ogre::Camera* mRttCam;
		Ogre::Viewport* mViewport;

		Ogre::ColourValue mBackgroungColour;

		Ogre::Real mSaveCamAspect;


		std::string mPointerKeeper;
		bool mLeftPressed;

		bool mAutoRotation;

		float mScale;
		float mCurrentScale;
		bool mUseScale;

		///
		Ogre::RealRect mViewportRect;
		Ogre::RealRect mRttCamSrcRect;

	}; // class TestRenderBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_TEST_RENDER_BOX_H__
