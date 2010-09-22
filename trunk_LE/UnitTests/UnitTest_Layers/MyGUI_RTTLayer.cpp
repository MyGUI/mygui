/*!
	@file
	@author		Albert Semenov
	@date		02/2008
*/

#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_FactoryManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

	RTTLayer::RTTLayer() :
		mTexture(nullptr)
	{
	}

	RTTLayer::~RTTLayer()
	{
		if (mTexture)
		{
			MyGUI::RenderManager::getInstance().destroyTexture(mTexture);
			mTexture = nullptr;
		}
	}

	void RTTLayer::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		MyGUI::xml::ElementEnumerator propert = _node->getElementEnumerator();
		while (propert.next("Property"))
		{
			const std::string& key = propert->findAttribute("key");
			const std::string& value = propert->findAttribute("value");
			if (key == "TextureSize") setTextureSize(utility::parseValue<IntSize>(value));
#ifdef MYGUI_OGRE_PLATFORM
			else if (key == "Entity") setEntity(value);
			else if (key == "Material") setMaterial(value);
			else if (key == "SceneManager") setSceneManager(value);
			else if (key == "Camera") setCamera(value);
#endif
		}
	}

	void RTTLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		bool out_date = false;

		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter)->castType<LayerNode>()->isOutOfDate())
			{
				out_date = true;
				break;
			}
		}

		if (out_date || _update)
		{
			MyGUI::IRenderTarget* target = mTexture->getRenderTarget();
			if (target != nullptr)
			{
				target->begin();

				for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
				{
					(*iter)->renderToTarget(target, _update);
				}

				target->end();
			}

		}
	}

	ILayerItem* RTTLayer::getLayerItemByPoint(int _left, int _top) const
	{
		if (!mIsPick)
			return nullptr;

#ifdef MYGUI_OGRE_PLATFORM
		const MyGUI::IntSize& size = MyGUI::RenderManager::getInstance().getViewSize();
		bool result = pickPositionInObject(_left, _top, size.width, size.height, mTextureSize.width, mTextureSize.height);
		if (result)
		{
			return Base::getLayerItemByPoint(_left, _top);
		}
#endif

		return nullptr;
	}

	IntPoint RTTLayer::getPosition(int _left, int _top) const
	{
		if (!mIsPick)
			return Base::getPosition(_left, _top);

#ifdef MYGUI_OGRE_PLATFORM
		const MyGUI::IntSize& size = MyGUI::RenderManager::getInstance().getViewSize();
		bool result = pickPositionInObject(_left, _top, size.width, size.height, mTextureSize.width, mTextureSize.height);
		if (result)
		{
			mOldPoint.set(_left, _top);
		}
#endif

		return mOldPoint;
	}

	void RTTLayer::setTextureSize(const IntSize& _size)
	{
		if (mTextureSize == _size) return;
		mTextureSize = _size;
		if (mTexture)
		{
			MyGUI::RenderManager::getInstance().destroyTexture(mTexture);
			mTexture = nullptr;
		}

		MYGUI_ASSERT(mTextureSize.width * mTextureSize.height, "RTTLayer texture size must have non-zero width and height");
		std::string name = MyGUI::utility::toString((size_t)this, getClassTypeName());
		mTexture = MyGUI::RenderManager::getInstance().createTexture(name);
		mTexture->createManual(mTextureSize.width, mTextureSize.height, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);

#ifdef MYGUI_OGRE_PLATFORM
		setTextureName(mTexture->getName());
#endif
	}

} // namespace MyGUI
