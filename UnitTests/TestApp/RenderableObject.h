/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __RENDERABLE_OBJECT_H__
#define __RENDERABLE_OBJECT_H__

#include "IRenderableObject.h"

namespace sim
{

	class RenderableObject : public IRenderableObject
	{
	public:
		MYGUI_RTTI_CHILD_HEADER(RenderableObject, IRenderableObject);

		RenderableObject(Ogre::SceneManager * _scene, const std::string& _meshname)
		{
			// создаем ентити
			mEntity = _scene->createEntity(MyGUI::utility::toString(this, "_", _meshname), _meshname);

			// устанавливаем тип смешивания костей
			Ogre::Skeleton * skeleton = mEntity->getSkeleton();
			if (skeleton) skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

			// прицепляем к ноду
			mNode = _scene->getRootSceneNode()->createChildSceneNode();
			mNode->attachObject(mEntity);
		}

		virtual Ogre::Entity * getEntity() { return mEntity; }
		virtual Ogre::SceneNode * getNode() { return mNode; }

	private:
		Ogre::Entity * mEntity;
		Ogre::SceneNode * mNode;
	};

} // namespace sim

#endif // __RENDERABLE_OBJECT_H__
