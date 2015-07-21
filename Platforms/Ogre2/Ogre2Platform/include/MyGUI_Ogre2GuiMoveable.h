#ifndef MYGUI_OGRE2_GUI_MOVEABLE_H_
#define MYGUI_OGRE2_GUI_MOVEABLE_H_

#include <Ogre.h>
#include <OgreSceneManager.h>

#include "MyGUI_Ogre2RenderManager.h"

namespace MyGUI 
{
	class Ogre2GuiMoveable : 
		public Ogre::MovableObject 
	{

	public:
		Ogre2GuiMoveable(Ogre::IdType id, Ogre::ObjectMemoryManager *objectMemoryManager,
						 Ogre::SceneManager* manager, Ogre::uint8 renderQueueId) :
			MovableObject(id, objectMemoryManager, manager, renderQueueId )
		{
		}

		virtual ~Ogre2GuiMoveable() 
		{
		}

		virtual const Ogre::String& getMovableType( void ) const 
		{
			return Ogre::BLANKSTRING;
		}

	};
}

#endif // MYGUI_OGRE_RENDER_MANAGER_H_