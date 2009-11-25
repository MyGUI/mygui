/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"
//#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include <Ogre.h>

namespace MyGUI
{
	Ogre::RaySceneQuery* gRaySceneQuery = 0;

	Ogre::SceneManager* getSceneManager()
	{
		return Ogre::Root::getSingleton().getSceneManager("BaseSceneManager");
	}

	Ogre::Camera* getCamera()
	{
		return getSceneManager()->getCamera("BaseCamera");
	}

	RTTLayer::RTTLayer() :
		mTexture(nullptr)
	{
		std::string name = MyGUI::utility::toString((int)this, "_RTTLayer");
		mTexture = MyGUI::RenderManager::getInstance().createTexture(name);
		mTexture->createManual(512, 512, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);

		Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName("Ground");
		material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(name);

		gRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
	}

	RTTLayer::~RTTLayer()
	{
		getSceneManager()->destroyQuery(gRaySceneQuery);
		gRaySceneQuery = nullptr;

		MyGUI::RenderManager::getInstance().destroyTexture(mTexture);
		mTexture = nullptr;
	}

	void RTTLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		MyGUI::IRenderTarget* target = mTexture->getRenderTarget();
		if (target != nullptr)
		{
			target->begin();

			for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
			{
				if (target != nullptr)
					(*iter)->renderToTarget(target, _update);
			}

			target->end();
		}
	}

	ILayerItem* RTTLayer::getLayerItemByPoint(int _left, int _top)
	{
		MyGUI::IntSize size = MyGUI::Gui::getInstance().getViewSize();
		Ogre::Ray ray = getCamera()->getCameraToViewportRay(
			_left / float(size.width),
			_top / float(size.height));
		gRaySceneQuery->setRay(ray);
		gRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult &result = gRaySceneQuery->execute();
		for (Ogre::RaySceneQueryResult::iterator iter = result.begin(); iter!=result.end(); ++iter)
		{
			if (iter->movable != 0)
			{
				if (iter->movable->getName() == "FloorPlane")
				{
					Ogre::Vector3 pos = ray.getPoint(iter->distance);
					//MyGUI::MYGUI_OUT(pos.x + 256, " - ", pos.z + 256);
					// нужно еще прибавлять смещение окна
					return Base::getLayerItemByPoint(512 - (pos.x + 256), 512 - (pos.z + 256));
				}
			}
		}

		return nullptr;
	}

} // namespace MyGUI
