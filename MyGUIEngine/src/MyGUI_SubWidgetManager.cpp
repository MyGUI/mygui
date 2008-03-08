/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_SubWidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(SubWidgetManager);

	void SubWidgetManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mFactoryMainSkin = new CroppedRectangleFactory<MainSkin>();
		registerFactory(mFactoryMainSkin);

		mFactorySubSkin = new CroppedRectangleFactory<SubSkin>();
		registerFactory(mFactorySubSkin);

		mFactoryTextEdit = new CroppedRectangleFactory<TextEdit>();
		registerFactory(mFactoryTextEdit);

		mFactoryTextSimple = new CroppedRectangleFactory<TextSimple>();
		registerFactory(mFactoryTextSimple);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SubWidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mFactoryList.clear();

		delete mFactoryMainSkin;
		delete mFactorySubSkin;
		delete mFactoryTextEdit;
		delete mFactoryTextSimple;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	CroppedRectangleInterface * SubWidgetManager::createSubWidget(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectangleInterface * _parent, size_t & _id)
	{
		for (std::list<SubWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createSubWidget(_info, _material, _parent, _id);
		}
		MYGUI_EXCEPT(_info.type << " - no find factory CroppedRectangleFactory");
		return 0;
	}

	// подходит ли данный тип для объединения
	bool SubWidgetManager::isSharedOverlay(const SubWidgetInfo &_info)
	{
		size_t id = 0;
		for (std::list<SubWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) {
				(*factory)->getNextId(id);
				break;
			}
		}
		return id != 0;
	}

} // namespace MyGUI
