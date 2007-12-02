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

		mFactoryTextEdit = new BasisWidgetFactory<TextEdit>();
		registerFactory(mFactoryTextEdit);

		mFactoryTextSimple = new BasisWidgetFactory<TextSimple>();
		registerFactory(mFactoryTextSimple);

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
		delete mFactoryTextEdit;
		delete mFactoryTextSimple;

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	BasisWidget * BasisWidgetManager::createBasisWidget(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidget * _parent, size_t & _id)
	{
		for (std::list<BasisWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createBasisWidget(_info, _material, _parent, _id);
		}
		MYGUI_EXCEPT(_info.type + " - no find factory BasisWidgetFactory");
		return 0;
	}

	// подходит ли данный тип для объединения
	bool BasisWidgetManager::isSharedOverlay(const BasisWidgetInfo &_info)
	{
		size_t id = 0;
		for (std::list<BasisWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) {
				(*factory)->getNextId(id);
				break;
			}
		}
		return id != 0;
	}

} // namespace MyGUI