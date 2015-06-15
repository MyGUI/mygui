#ifndef MYGUI_OGRE21_GUI_MOVEABLE_H_
#define MYGUI_OGRE21_GUI_MOVEABLE_H_

#include <Ogre.h>
#include <OgreSceneManager.h>

#include "MyGUI_Ogre21RenderManager.h"

namespace MyGUI 
{
	class Ogre21GuiMoveable : 
		public Ogre::MovableObject 
	{

	public:
		Ogre21GuiMoveable(Ogre::SceneManager* _scene) : 
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

		~Ogre21GuiMoveable() {

		}

		virtual const Ogre::String& getMovableType( void ) const {
			return Ogre::BLANKSTRING;
		}

	};
}

#endif // MYGUI_OGRE_RENDER_MANAGER_H_