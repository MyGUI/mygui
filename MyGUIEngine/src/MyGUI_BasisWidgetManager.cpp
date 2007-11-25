/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_BasisWidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(BasisWidgetManager);

	void BasisWidgetManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		mFactoryMainSkin = new BasisWidgetFactory<MainSkin>();
		registerFactory(mFactoryMainSkin);

		mFactorySubSkin = new BasisWidgetFactory<SubSkin>();
		registerFactory(mFactorySubSkin);

		mFactoryTextSimple = new BasisWidgetFactory<TextSimple>();
		registerFactory(mFactoryTextSimple);

		mFactoryTextEdit = new BasisWidgetFactory<TextEdit>();
		registerFactory(mFactoryTextEdit);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void BasisWidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		mFactoryList.clear();

		delete mFactoryMainSkin;
		delete mFactorySubSkin;
		delete mFactoryTextSimple;
		delete mFactoryTextEdit;

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	BasisWidget * BasisWidgetManager::createBasisWidget(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidget * _parent)
	{
		for (std::list<BasisWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent);
		}
		MYGUI_EXCEPT(_info.type + " - no find factory BasisWidgetFactory");
		return 0;
	}

} // namespace MyGUI