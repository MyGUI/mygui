/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "TextureViewToolControl.h"
#include "FactoryManager.h"
#include "IControlController.h"
#include "ScopeManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TextureViewToolControl)

	TextureViewToolControl::TextureViewToolControl() :
		mTextureTool(nullptr),
		mPlace(nullptr),
		mGroupController(nullptr),
		mActivated(false)
	{
	}

	TextureViewToolControl::~TextureViewToolControl()
	{
		delete mGroupController;
	}

	void TextureViewToolControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mPlace, "Place");

		mTextureTool = new ScopeTextureControl();
		mTextureTool->Initialise(this, mPlace, "");

		IFactoryItem* item = components::FactoryManager::GetInstance().CreateItem("GroupTextureController");
		if (item != nullptr)
		{
			mGroupController = dynamic_cast<IControlController*>(item);
			if (mGroupController != nullptr)
			{
				mGroupController->setTarget(mTextureTool);
			}
			else
			{
				delete item;
			}
		}

		ScopeManager::getInstance().eventChangeScope.connect(this, &TextureViewToolControl::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void TextureViewToolControl::notifyChangeScope(const std::string& _scope)
	{
		if (_scope == "Group")
		{
			if (!mActivated)
			{
				if (mGroupController != nullptr)
					mGroupController->activate();
				mActivated = true;
			}
		}
		else
		{
			if (mActivated)
			{
				if (mGroupController != nullptr)
					mGroupController->diactivate();
				mActivated = false;
			}
		}
	}

}
