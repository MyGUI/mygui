/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_BasisWidgetManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(CroppedRectangleManager);

	void CroppedRectangleManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

		mFactoryMainSkin = new CroppedRectangleFactory<MainSkin>();
		registerFactory(mFactoryMainSkin);

		mFactorySubSkin = new CroppedRectangleFactory<SubSkin>();
		registerFactory(mFactorySubSkin);

		mFactoryTextEdit = new CroppedRectangleFactory<TextEdit>();
		registerFactory(mFactoryTextEdit);

		mFactoryTextSimple = new CroppedRectangleFactory<TextSimple>();
		registerFactory(mFactoryTextSimple);

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void CroppedRectangleManager::shutdown()
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

	CroppedRectangleInterface * CroppedRectangleManager::createCroppedRectangle(const CroppedRectangleInfo &_info, const Ogre::String & _material, CroppedRectangleInterface * _parent, size_t & _id)
	{
		for (std::list<CroppedRectangleFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createCroppedRectangle(_info, _material, _parent, _id);
		}
		MYGUI_EXCEPT(_info.type + " - no find factory CroppedRectangleFactory");
		return 0;
	}

	// подходит ли данный тип для объединения
	bool CroppedRectangleManager::isSharedOverlay(const CroppedRectangleInfo &_info)
	{
		size_t id = 0;
		for (std::list<CroppedRectangleFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) {
				(*factory)->getNextId(id);
				break;
			}
		}
		return id != 0;
	}

} // namespace MyGUI