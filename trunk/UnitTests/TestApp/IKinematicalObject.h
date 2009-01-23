/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_KINEMATICAL_OBJECT_H__
#define __I_KINEMATICAL_OBJECT_H__

#include "IBase.h"
#include <OIS/OIS.h>

namespace sim
{

	class IKinematicalObject : public IBase
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(IKinematicalObject, IBase);

		virtual bool IsMoving() = 0;

		virtual void keyPressed(OIS::KeyCode _key) = 0;
		virtual void keyReleased(OIS::KeyCode _key) = 0;

		virtual ~IKinematicalObject() { }

	};

} // namespace sim

#endif // __I_KINEMATICAL_OBJECT_H__
