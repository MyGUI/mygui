/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __RENDER_BOX_WRAP_H__
#define __RENDER_BOX_WRAP_H__

#include <Ogre.h>
#include <MyGUI.h>

namespace wraps
{

	class RenderBoxWrap
	{
	public:
		RenderBoxWrap(MyGUI::RenderBoxPtr _box);
		~RenderBoxWrap();

		/** default RenderBox autorotation speed (if enabled) */
		enum { RENDER_BOX_AUTO_ROTATION_SPEED = 20 };


		/** Add mesh to scene and remove previous one
			@remarks
				This function will take no effect if setRenderTarget was used.
			@param
				_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position = Ogre::Vector3::ZERO, const Ogre::Quaternion & _orientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 & _scale = Ogre::Vector3::UNIT_SCALE);

		/** Add scene node to scene and remove previous one
		@remarks
		This function will take no effect if setRenderTarget was used.
		@param
		_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectSceneNode(Ogre::SceneManager * _manager, Ogre::SceneNode* _sceneNode);

		/** Run mesh animation if animation with such name exist (else print warning in log).
			To stop animation use empty string.
		*/
		void setAnimation(const Ogre::String& _animation);

		/** Clear scene */
		void clearScene();

		/** Set speed of entity rotation.
			@remarks
				This function will take no effect if setRenderTarget was used.
			@param
				_speed of rotation in degrees per second.
		*/
		void setAutoRotationSpeed(int _speed = RENDER_BOX_AUTO_ROTATION_SPEED);
		/** Get speed of entity rotation.*/
		int getAutoRotationSpeed() { return mRotationSpeed; }

		/** Enable or disable auto rotation
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setAutoRotation(bool _auto);
		/** Get auto rotation flag */
		bool getAutoRotation() { return mAutoRotation; }

		/** Set colour behind entity.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setBackgroungColour(const Ogre::ColourValue& _backgroundColour) { mRenderBox->setBackgroungColour(_backgroundColour); }
		/** Get colour behind entity.*/
		const Ogre::ColourValue& getBackgroungColour() { return mRenderBox->getBackgroungColour(); }

		/** Set start rotation angle of entity.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setRotationAngle(const Ogre::Degree & _rotationAngle);

		/** Get rotation angle of entity.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		Ogre::Degree getRotationAngle();

		/** Set possibility to rotate mesh by mouse drag.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setMouseRotation(bool _enable);
		/** Get mouse rotation flag */
		bool getMouseRotation() { return mMouseRotation; }

		/** Set possibility to zoom mesh by mouse wheel.
			@remarks
				This function will take no effect if setRenderTarget was used.
		*/
		void setViewScale(bool _scale);
		/** Get possibility to zoom mesh by mouse wheel flag. */
		bool getViewScale() { return mUseScale; }

		bool getScreenPosition(const Ogre::Vector3 _world, Ogre::Vector2& _screen);

	protected:

		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);

		void notifyUpdateViewport(MyGUI::RenderBoxPtr _sender);

	private:
		void frameEntered(float _time);

		void synchronizeSceneNode(Ogre::SceneNode* _newNode, Ogre::SceneNode* _fromNode);
		bool checkSceneNode(Ogre::SceneNode * _sceneNode, Ogre::SceneNode * _node);
		bool needFrameUpdate() { return mAutoRotation || mUseScale || (nullptr != mEntityState) || (mNodeForSync != nullptr); }
		void createScene();
		void updateViewport();

		void removeNode(Ogre::SceneNode* _node);
		void removeEntity(const Ogre::String& _name);

		void FrameAdvise(bool _advise);

	private:
		MyGUI::RenderBoxPtr mRenderBox;

		Ogre::SceneManager * mScene;
		Ogre::SceneNode * mNode;
		Ogre::Camera* mRttCam;
		Ogre::SceneNode* mCamNode;

		Ogre::Entity * mEntity;
		Ogre::AnimationState * mEntityState;

		typedef std::vector<Ogre::Entity*> VectorEntity;
		VectorEntity mVectorEntity;

		float mSyncTime;
		Ogre::SceneNode* mNodeForSync;
		Ogre::SceneManager * mSceneManagerForSync;

		int mRotationSpeed;
		bool mMouseRotation;
		int mLastPointerX;
		bool mLeftPressed;
		bool mAutoRotation;

		float mScale;
		float mCurrentScale;
		bool mUseScale;
		bool mFrameAdvise;

	};

} // namespace wraps

#endif // __RENDER_BOX_WRAP_H__
