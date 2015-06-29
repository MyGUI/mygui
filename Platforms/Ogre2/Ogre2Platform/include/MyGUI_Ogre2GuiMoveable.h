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
		Ogre2GuiMoveable(Ogre::SceneManager* _scene) : 
			MovableObject(Ogre::Id::generateNewId<Ogre::MovableObject>() , 
				&_scene->_getEntityMemoryManager( Ogre::SCENE_DYNAMIC ) , 
				_scene , 
				254 )
		{
			
			Ogre::NodeMemoryManager* mNodeMemoryManager = new Ogre::NodeMemoryManager();

			Ogre::SceneNode* mDummyNode = OGRE_NEW Ogre::SceneNode( 0, 0, mNodeMemoryManager, 0 );
			mDummyNode->_getFullTransformUpdated();
			mDummyNode->attachObject( this );

		}

		~Ogre2GuiMoveable() {

		}

		virtual const Ogre::String& getMovableType( void ) const {
			return Ogre::BLANKSTRING;
		}

	};
}

#endif // MYGUI_OGRE_RENDER_MANAGER_H_