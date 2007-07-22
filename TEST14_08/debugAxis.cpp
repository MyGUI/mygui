	//---------------------------------------------------------------------------------//
	// debugAxis.cpp
	// Мировые оси для отладки сцены [my.name (my.work(a)inbox.ru)]
	//---------------------------------------------------------------------------------//

	#include "debugAxis.h"

	namespace debugAxis
	{
		// информация об осях
		#define __MAX_AXIS_LEN 5000
		#define __COLOR_AXIS_X ColourValue(1,0,0)
		#define __COLOR_AXIS_Y ColourValue(0,1,0)
		#define __COLOR_AXIS_Z ColourValue(0,0,1)

		using namespace Ogre;

		SceneNode* g_nodeDebugAxis = NULL;

		void create(Ogre::SceneManager* mSceneMgr)
		{

			if (!mSceneMgr) return;
			if (g_nodeDebugAxis != NULL) return;

			try {
				g_nodeDebugAxis = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeDebugAxis");

				MaterialPtr materialAxisX = MaterialManager::getSingleton().create("materialDebugAxisX", "debugger"); 
				materialAxisX->setReceiveShadows(false); 
				materialAxisX->getTechnique(0)->setLightingEnabled(true); 
				materialAxisX->getTechnique(0)->getPass(0)->setDiffuse(__COLOR_AXIS_X); 
				materialAxisX->getTechnique(0)->getPass(0)->setAmbient(__COLOR_AXIS_X); 
				materialAxisX->getTechnique(0)->getPass(0)->setSelfIllumination(__COLOR_AXIS_X); 

				MaterialPtr materialAxisY = MaterialManager::getSingleton().create("materialDebugAxisY", "debugger"); 
				materialAxisY->setReceiveShadows(false); 
				materialAxisY->getTechnique(0)->setLightingEnabled(true); 
				materialAxisY->getTechnique(0)->getPass(0)->setDiffuse(__COLOR_AXIS_Y);
				materialAxisY->getTechnique(0)->getPass(0)->setAmbient(__COLOR_AXIS_Y); 
				materialAxisY->getTechnique(0)->getPass(0)->setSelfIllumination(__COLOR_AXIS_Y); 

				MaterialPtr materialAxisZ = MaterialManager::getSingleton().create("materialDebugAxisZ", "debugger"); 
				materialAxisZ->setReceiveShadows(false); 
				materialAxisZ->getTechnique(0)->setLightingEnabled(true); 
				materialAxisZ->getTechnique(0)->getPass(0)->setDiffuse(__COLOR_AXIS_Z);
				materialAxisZ->getTechnique(0)->getPass(0)->setAmbient(__COLOR_AXIS_Z);
				materialAxisZ->getTechnique(0)->getPass(0)->setSelfIllumination(__COLOR_AXIS_Z);

				ManualObject* lineAxis =  mSceneMgr->createManualObject("lineDubugAxis");
				lineAxis->setCastShadows(false);

				lineAxis->begin("materialDebugAxisX", Ogre::RenderOperation::OT_LINE_LIST); 
				lineAxis->position(0, 0, 0);
				lineAxis->position(__MAX_AXIS_LEN, 0, 0); 
				lineAxis->end(); 

				lineAxis->begin("materialDebugAxisY", Ogre::RenderOperation::OT_LINE_LIST); 
				lineAxis->position(0, 0, 0);
				lineAxis->position(0, __MAX_AXIS_LEN, 0); 
				lineAxis->end(); 

				lineAxis->begin("materialDebugAxisZ", Ogre::RenderOperation::OT_LINE_LIST); 
				lineAxis->position(0, 0, 0);
				lineAxis->position(0, 0, __MAX_AXIS_LEN); 
				lineAxis->end(); 

				g_nodeDebugAxis->attachObject(lineAxis);
				g_nodeDebugAxis->setVisible(false);

			} catch (...) {}

		}

		void show(bool _show)
		{
			if (g_nodeDebugAxis == NULL) return;
			try { g_nodeDebugAxis->setVisible(_show); } catch(...){}
		}

	} // namespace debugAxis
	//---------------------------------------------------------------------------------//
