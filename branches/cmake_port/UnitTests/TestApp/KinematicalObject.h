/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __KINEMATICAL_OBJECT_H__
#define __KINEMATICAL_OBJECT_H__

#include "IBase.h"
#include "IKinematicalObject.h"
#include "IAnimationGraph.h"
#include "TimeContext.h"
#include  "AnimatebleObject.h"

namespace sim
{

	class KinematicalObject : public IKinematicalObject
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(KinematicalObject, IKinematicalObject);

		KinematicalObject() :
			mWalk(false)
		{
		}
		virtual bool IsMoving() { return mWalk; } 

		virtual ~KinematicalObject() { }

		virtual void keyPressed(OIS::KeyCode _key)
		{
			if (_key == OIS::KC_W) {
				mWalk = true;
			}
			else if (_key == OIS::KC_SPACE) {
			}
		}

		virtual void keyReleased(OIS::KeyCode _key)
		{
			if (_key == OIS::KC_W) {
				mWalk = false;
			}
		}

	private:
		bool mWalk;
	};

} // namespace sim

#endif // __KINEMATICAL_OBJECT_H__
