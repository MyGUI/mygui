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

namespace MyGUI
{
	/** default RenderBox autorotation speed (if enabled) */
	const size_t RENDER_BOX_AUTO_ROTATION_SPEED = 20;

	/** @brief Widget that show one entity or anything from viewport.

		This widget can show autorotaded and rotatable by mouse mesh.
		Also you can set your own Ogre::Camera and yo'll see anything from your viewport.
	*/
	class _MyGUIExport RenderBox : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<RenderBox>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		RenderBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~RenderBox();

	public:
		/** Add mesh to scene and remove previous one
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
			@param
				_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position = Ogre::Vector3::ZERO, const Ogre::Quaternion & _orientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 & _scale = Ogre::Vector3::UNIT_SCALE);

		/** Add scene node to scene and remove previous one
		@remarks
		This function will take no effect if user Viewport provided via setViewport.
		@param
		_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectSceneNode(Ogre::SceneNode* _sceneNode);

		void synchronizeSceneNode(Ogre::SceneNode* _newNode, Ogre::SceneNode* _fromNode);

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
		int getAutoRotationSpeed() {return mRotationSpeed;};

		/** Enable or disable auto rotation
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setAutoRotation(bool _auto);
		/** Get auto rotation flag */
		bool getAutoRotation() {return mAutoRotation;}

		/** Set colour behind entity.
			@remarks
				This function will take no effect if user Viewport provided via setViewport.
		*/
		void setBackgroungColour(const Ogre::ColourValue & _backgroundColour);
		/** Get colour behind entity.*/
		const Ogre::ColourValue & getBackgroungColour() {return mBackgroungColour;};

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
		bool getViewScale() {return mUseScale;}

		/** Set any user created Camera instead of showing one mesh*/
		void setRenderTarget(Ogre::Camera * _camera);

		bool getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen);

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

	protected:
		void frameEntered(float _time);

		void onMouseDrag(int _left, int _top);
		void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		void onMouseWheel(int _rel);

	private:
		bool needFrameUpdate() {return mAutoRotation || mUseScale || (null != mEntityState) || (mNodeForSync != null);}
		void createRenderTexture();
		void updateViewport();

		void removeNode(Ogre::SceneNode* _node);
		void removeEntity(const Ogre::String& _name);

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

		float mSyncTime;
		Ogre::SceneNode* mNodeForSync;

	}; // class RenderBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_H__
