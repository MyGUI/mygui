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
	/** default RenderBox autorotation speed (if enabled) */
	const size_t RENDER_BOX_AUTO_ROTATION_SPEED = 20;

	/** @brief Widget that show one entity or anything from viewport.

		This widget can show autorotaded and rotatable by mouse mesh.
		Also you can set your own Ogre::Camera and yo'll see anything from your viewport.
	*/
	class _MyGUIExport RenderBox : public Widget, public FrameListener
	{
		// для вызова закрытого конструктора
		friend class factory::RenderBoxFactory;

	protected:
		RenderBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~RenderBox();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Add mesh to scene and remove previous one
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
			@param
				_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position = Ogre::Vector3::ZERO, const Ogre::Quaternion & _orientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 & _scale = Ogre::Vector3::UNIT_SCALE);

		/** Run mesh animation if animation with such name exist (else print warning in log).
			To stop animation use empty string.
		*/
		void setAnimation(const Ogre::String& _animation);

		/** Clear scene */
		void clear();

		/** Set speed of entity rotation.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
			@param
				_speed of rotation in degrees per second.
		*/
		void setAutoRotationSpeed(int _speed = RENDER_BOX_AUTO_ROTATION_SPEED);
		/** Get speed of entity rotation.*/
		inline int getAutoRotationSpeed() {return mRotationSpeed;};

		/** Enable or disable auto rotation
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setAutoRotation(bool _auto);
		/** Get auto rotation flag */
		inline bool getAutoRotation() {return mAutoRotation;}

		/** Set colour behind entity.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setBackgroungColour(const Ogre::ColourValue & _backgroundColour);
		/** Get colour behind entity.*/
		inline const Ogre::ColourValue & getBackgroungColour() {return mBackgroungColour;};

		/** Set start rotation angle of entity.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setRotationAngle(const Ogre::Degree & _rotationAngle);

		/** Get rotation angle of entity.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		Ogre::Degree getRotationAngle();

		/** Set possibility to rotate mesh by mouse drag.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setMouseRotation(bool _enable);
		/** Get mouse rotation flag */
		bool getMouseRotation() {return mMouseRotation;};

		/** Set possibility to zoom mesh by mouse wheel.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setViewScale(bool _scale);
		/** Get possibility to zoom mesh by mouse wheel flag. */
		inline bool getViewScale() {return mUseScale;}

		/** Set any user created Camera instead of showing one mesh*/
		void setRenderTarget(Ogre::Camera * _camera);

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

	protected:
		void _frameEntered(float _time);

		void _onMouseDrag(int _left, int _top);
		void _onMouseButtonPressed(int _left, int _top, MouseButton _id);
		void _onMouseButtonReleased(int _left, int _top, MouseButton _id);
		void _onMouseWheel(int _rel);

	private:
		inline bool needFrameUpdate() {return mAutoRotation || mUseScale || (null != mEntityState);}
		void createRenderTexture();
		void updateViewport();

		bool mUserViewport;
		// все, что касается сцены
		Ogre::SceneManager * mScene;
		Ogre::Entity * mEntity;
		Ogre::SceneNode * mNode;
		Ogre::RenderTexture* mTexture;

		Ogre::Camera* mRttCam;
		Ogre::SceneNode* mCamNode;

		int mRotationSpeed;
		Ogre::ColourValue mBackgroungColour;
		bool mMouseRotation;
		int mLastPointerX;

		std::string mPointerKeeper;
		bool mLeftPressed;

		bool mAutoRotation;

		Ogre::AnimationState * mEntityState;

		float mScale, mCurrentScale;
		bool mUseScale;

		typedef std::vector<Ogre::Entity*> VectorEntity;
		VectorEntity mVectorEntity;

	}; // class RenderBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_H__
