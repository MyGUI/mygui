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

		mFactorySubSkin = new CroppedRectangleFactory<SubSkin>("SubSkin");
		mFactoryMainSkin = new CroppedRectangleFactory<MainSkin>("MainSkin");
		mFactorySimpleText = new CroppedRectangleFactory<SimpleText>("SimpleText");
		mFactoryEditText = new CroppedRectangleFactory<EditText>("EditText");
		mFactoryColourRect = new CroppedRectangleFactory<ColourRect>("ColourRect");

		registerFactory(mFactorySubSkin);
		registerFactory(mFactoryMainSkin);
		registerFactory(mFactorySimpleText);
		registerFactory(mFactoryEditText);
		registerFactory(mFactoryColourRect);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void SubWidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		mFactoryList.clear();

		delete mFactorySubSkin;
		delete mFactoryMainSkin;
		delete mFactorySimpleText;
		delete mFactoryEditText;
		delete mFactoryColourRect;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	CroppedRectangleInterface * SubWidgetManager::createSubWidget(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent)
	{
		for (std::list<SubWidgetFactoryInterface*>::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ((*factory)->getType() == _info.type) return (*factory)->createSubWidget(_info, _parent);
		}
		MYGUI_EXCEPT(_info.type << " - no find factory CroppedRectangleFactory");
		return 0;
	}

} // namespace MyGUI
