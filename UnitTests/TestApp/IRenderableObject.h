/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_RENDERABLE_OBJECT_H__
#define __I_RENDERABLE_OBJECT_H__

#include <Ogre.h>
#include "IBase.h"

namespace sim
{

	class IRenderableObject : public IBase
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(IRenderableObject, IBase);

		virtual Ogre::Entity * getEntity() = 0;
		virtual Ogre::SceneNode * getNode() = 0;

	};

} // namespace sim

#endif // __RENDERABLE_OBJECT_H__
