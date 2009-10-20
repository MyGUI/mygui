/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
/*
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
#include "RenderBox.h"

namespace wraps
{

	class RenderBoxScene : public RenderBox
	{
	public:
		RenderBoxScene();
		virtual ~RenderBoxScene();

		/** default RenderBox autorotation speed (if enabled) */
		enum { RENDER_BOX_AUTO_ROTATION_SPEED = 20 };


		/** Add mesh to scene and remove previous one
			@param
				_meshName The name of the Mesh it is to be based on (e.g. 'ogrehead.mesh').
		*/
		void injectObject(const Ogre::String& _meshName, const Ogre::Vector3 & _position = Ogre::Vector3::ZERO, const Ogre::Quaternion & _orientation = Ogre::Quaternion::IDENTITY, const Ogre::Vector3 & _scale = Ogre::Vector3::UNIT_SCALE);

		/** Run mesh animation if animation with such name exist (else print warning in log).
			To stop animation use empty string.
		*/
		void setAnimation(const Ogre::String& _animation);

		/** Clear scene */
		void clearScene();

		/** Set speed of entity rotation.
			@param
				_speed of rotation in degrees per second.
		*/
		void setAutoRotationSpeed(int _speed = RENDER_BOX_AUTO_ROTATION_SPEED);
		/** Get speed of entity rotation.*/
		int getAutoRotationSpeed() { return mRotationSpeed; }

		/** Enable or disable auto rotation */
		void setAutoRotation(bool _value);
		/** Get auto rotation flag */
		bool getAutoRotation() { return mAutoRotation; }

		/** Set start rotation angle of entity. */
		void setRotationAngle(const Ogre::Degree& _rotationAngle);

		/** Get rotation angle of entity. */
		Ogre::Degree getRotationAngle();

		/** Set possibility to rotate mesh by mouse drag. */
		void setMouseRotation(bool _enable);
		/** Get mouse rotation flag */
		bool getMouseRotation() { return mMouseRotation; }

		virtual void setCanvas(MyGUI::CanvasPtr _value);

		virtual void destroy();

	private:
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);

		void frameAdvise(bool _advise);
		void frameEntered(float _time);

		bool needFrameUpdate() { return mAutoRotation || mAnimationState; }
		void createScene();
		void updateViewport();

		virtual void requestUpdateCanvas(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event);

	private:
		Ogre::SceneManager* mScene;
		Ogre::SceneNode* mNode;
		Ogre::SceneNode* mCameraNode;
		Ogre::Camera* mCamera;
		Ogre::Entity* mEntity;
		Ogre::AnimationState* mAnimationState;

		int mRotationSpeed;
		bool mMouseRotation;
		int mLastPointerX;
		bool mLeftPressed;
		bool mAutoRotation;

		bool mFrameAdvise;
	};

} // namespace wraps

#endif // __RENDER_BOX_WRAP_H__
