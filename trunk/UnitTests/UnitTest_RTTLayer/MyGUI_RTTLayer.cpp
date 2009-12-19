/*!
	@file
	@author		Albert Semenov
	@date		12/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_Common.h"
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
			if (key == "TextureName") setTextureName(value);
		}
	}

	void RTTLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		bool out_date = false;

		for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
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

				for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
				{
					(*iter)->renderToTarget(target, _update);
				}

				target->end();
			}

		}
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
		std::string name = mTextureName.empty() ? MyGUI::utility::toString((size_t)this, getClassTypeName()) : mTextureName;
		mTexture = MyGUI::RenderManager::getInstance().createTexture(name);
		mTexture->createManual(mTextureSize.width, mTextureSize.height, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
	}

	void RTTLayer::setTextureName(const std::string& _name)
	{
		mTextureName = _name;
		if (mTexture != nullptr)
		{
			IntSize size = mTextureSize;
			mTextureSize.clear();
			setTextureSize(size);
		}
	}

} // namespace MyGUI
