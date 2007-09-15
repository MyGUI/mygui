	//---------------------------------------------------------------------------------//
	// debugAxis.h
	// Мировые оси для отладки сцены [my.name (my.work(a)inbox.ru)]
	//---------------------------------------------------------------------------------//
	#ifndef __debugAxis_h__
	#define __debugAxis_h__

	#include <Ogre.h>

	namespace debugAxis
	{

		void create(Ogre::SceneManager* mSceneMgr);
		void show(bool _show);

	} // namespace debugAxis

	#endif // #ifndef __debugAxis_h__
	//---------------------------------------------------------------------------------//
