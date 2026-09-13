/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "Precompiled.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_RTTLayer.h"

#include "MyGUI_FactoryManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

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
			std::string_view key = propert->findAttribute("key");
			std::string_view value = propert->findAttribute("value");
			if (key == "TextureSize")
				setTextureSize(utility::parseValue<IntSize>(value), Gui::getInstance().getDpiScale());
			if (key == "TextureName")
				setTextureName(value);
		}
	}

	void RTTLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		bool outOfDate = mOutOfDateRtt || isOutOfDate();

		if (outOfDate || _update)
		{
			MyGUI::IRenderTarget* target = mTexture->getRenderTarget();
			if (target != nullptr)
			{
				target->begin();

				for (auto& childItem : mChildItems)
					childItem->renderToTarget(target, _update);

				target->end();
			}
		}

		mOutOfDateRtt = false;
	}

	void RTTLayer::setTextureSize(const IntSize& _size, float _dpiScale)
	{
		if (mTextureSize == _size && mDpiScale == _dpiScale)
			return;

		mTextureSize = _size;
		mDpiScale = _dpiScale;
		if (mTexture)
		{
			MyGUI::RenderManager::getInstance().destroyTexture(mTexture);
			mTexture = nullptr;
		}

		MYGUI_ASSERT(
			mTextureSize.width && mTextureSize.height,
			"RTTLayer texture size must have non-zero width and height");

		int physicalWidth = (int)(mTextureSize.width * mDpiScale);
		int physicalHeight = (int)(mTextureSize.height * mDpiScale);

		std::string name =
			mTextureName.empty() ? MyGUI::utility::toString((size_t)this, getClassTypeName()) : mTextureName;
		mTexture = MyGUI::RenderManager::getInstance().createTexture(name);
		mTexture->createManual(
			physicalWidth,
			physicalHeight,
			MyGUI::TextureUsage::RenderTarget,
			MyGUI::PixelFormat::R8G8B8A8);

		IRenderTarget* target = mTexture->getRenderTarget();
		if (target != nullptr)
		{
			RenderTargetInfo& info = const_cast<RenderTargetInfo&>(target->getInfo());
			info.pixScaleX = 1.0f / mTextureSize.width;
			info.pixScaleY = 1.0f / mTextureSize.height;
		}

		mOutOfDateRtt = true;
	}

	void RTTLayer::setTextureName(std::string_view _name)
	{
		mTextureName = _name;

		if (mTexture != nullptr)
		{
			IntSize size = mTextureSize;
			mTextureSize.clear();
			setTextureSize(size, mDpiScale);
		}

		mOutOfDateRtt = true;
	}

} // namespace MyGUI
